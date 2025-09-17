// TickToucan.h
#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>

//------------------------------------------------------------------------------
// 1. Configuration: adjust this for your MCU
//------------------------------------------------------------------------------
static constexpr std::size_t MAX_TASKS = 16;

// The tick type (e.g. milliseconds since reset)
using TickType = uint64_t;

//------------------------------------------------------------------------------
// 2. Task Structure & Singleton Scheduler
//------------------------------------------------------------------------------
class TickToucan {

public:

    using Callback = void(*)(void*);

    // Return a handle (or -1) when scheduling
    struct Handle { int idx; };

    TickToucan(const TickToucan&) = delete;

    TickToucan& operator=(const TickToucan&) = delete; 
    
    ~TickToucan();

    /**
     * @brief Get the singleton instance of TickToucan.
     * @return Reference to the TickToucan instance.
     */
    static TickToucan& instance();

    /**
     * @brief Initialize the scheduler with the tick period in milliseconds.
     * @param tick_ms Tick period in milliseconds.
     */
    void init(uint32_t tick_ms);

    /**
     * @brief Trampoline function to be called by the timer ISR to increment the tick.
     * @note  Used internally; do not call directly.
     */
    static void tickTrampoline();

    /**
     * @brief Run all ready callbacks. Call this in the main loop.
     */
    void dispatch();

    /**
     * @brief Schedule a one-shot callback at an absolute tick.
     * @note All time arguments specified in milliseconds will be internally converted to ticks based on the configured tick period. If the conversion does not result in an exact tick value, the nearest lower tick will be used.
     * @param absolute_time_ms Absolute tick (in ms) to schedule the callback.
     * @param cb Callback function to execute.
     * @param ctx Optional user context pointer.
     * @return Handle to the scheduled task.
     */
    TickToucan::Handle scheduleAt(TickType absolute_time_ms, Callback cb, void* ctx = nullptr);

    /**
     * @brief Schedule a periodic callback. First run at now + interval + offset, then every interval.
     * @note All time arguments specified in milliseconds will be internally converted to ticks based on the configured tick period. If the conversion does not result in an exact tick value, the nearest lower tick will be used.
     * @param interval_time_ms Interval between executions in ms.
     * @param cb Callback function to execute.
     * @param ctx Optional user context pointer.
     * @param t_offset_ms Optional offset for the first execution in ms.
     * @return Handle to the scheduled task.
     */
    TickToucan::Handle scheduleEvery(TickType interval_time_ms, Callback cb, void* ctx = nullptr, TickType t_offset_ms = 0);

    /**
     * @brief Cancel a pending task.
     * @param h Handle to the task to cancel.
     */
    void cancel(Handle h);

    /**
     * @brief Get the current tick count.
     * @return Current tick count.
     */
    TickType now() const;

private:


    unsigned int m_tick_ms = 0; // tick period in ms

    bool m_initialized = false;

    struct Task
    {
        bool     active;       // is slot in use?
        TickType nextTick;     // absolute tick to fire
        TickType interval;     // 0 = one-shot; >0 = periodic
        Callback cb;           // function to call
        void*    ctx;          // user data for callback
        volatile bool ready;   // set to true by ISR when due

        Task();
        
    };    

    volatile TickType currentTick_;

    Task tasks_[MAX_TASKS];

    static std::shared_ptr<TickToucan> s_pTickToucan;

    TickToucan();

    bool valid(Handle h) const;

    TickToucan::Handle addTask(TickType next, TickType interval, Callback cb, void* ctx);

    void incrementTick();

    void scheduleTask();

    void tickISR();

    TickType convertMsToTicks(TickType t_ms);


};