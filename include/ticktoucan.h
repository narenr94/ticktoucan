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
using TickType = uint32_t;

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

    // Get the singleton
    static TickToucan& instance();

    // call me once, early in main()
    void init(uint32_t tick_ms);

    // to be called by timer ISR to inclrement tick
    static void tickTrampoline();

    // Call in main loop to run all ready callbacks
    void dispatch();

    // Schedule a one-shot at absolute tick
    TickToucan::Handle scheduleAt(TickType absoluteTick, Callback cb, void* ctx = nullptr);

    // Schedule first run at now+interval and repeat every 'interval'
    TickToucan::Handle scheduleEvery(TickType interval, Callback cb, void* ctx = nullptr, TickType t_offset = 0);

    // Cancel a pending task
    void cancel(Handle h);

    // Get current tick count
    TickType now() const;

private:


    unsigned int m_tick_ms = 0; // tick period in ms   

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
  



};