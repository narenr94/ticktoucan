// TickToucan.h
#pragma once

#include <cstdint>
#include <cstddef>

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

private:

    using Callback = void(*)(void*);

    struct Task {
        bool     active;       // is slot in use?
        TickType nextTick;     // absolute tick to fire
        TickType interval;     // 0 = one-shot; >0 = periodic
        Callback cb;           // function to call
        void*    ctx;          // user data for callback
        volatile bool ready;   // set to true by ISR when due
    };

    // Return a handle (or -1) when scheduling
    struct Handle { int idx; };

    volatile TickType currentTick_;

    Task tasks_[MAX_TASKS];

    BareMetalScheduler();

    bool valid(Handle h) const;

    Handle addTask(TickType next, TickType interval, Callback cb, void* ctx);
  

public:

    ~BareMetalScheduler();

    // Get the singleton
    static TickToucan& instance();

    // Called in your timer ISR (e.g. SysTick_Handler)
    void tickISR();

    // Call in main loop to run all ready callbacks
    void dispatch();

    // Schedule a one-shot at absolute tick
    Handle scheduleAt(TickType absoluteTick, Callback cb, void* ctx = nullptr);

    // Schedule first run at now+interval and repeat every 'interval'
    Handle scheduleEvery(TickType interval, Callback cb, void* ctx = nullptr);

    // Cancel a pending task
    void cancel(Handle h);

    // Get current tick count
    TickType now() const;

};