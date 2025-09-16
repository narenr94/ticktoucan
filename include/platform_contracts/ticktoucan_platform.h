//ticktoucan_platform.h

#pragma once

#include <cstdint>


//disables interrupts and returns current mask to re-enable later
uint32_t platform_enter_critical();

//enables interrupts using mask
void platform_exit_critical(uint32_t primask);

// Install a millisecond tick timer that calls `tickCb()` every tick_ms.
void platform_setup_tick_timer(uint32_t tick_ms, void (*tickCb)());

// Stops and cleans up the tick timer
void platform_cleanup_tick_timer();

#define ENTER_CRITICAL()  platform_enter_critical()
#define EXIT_CRITICAL(p)  platform_exit_critical(p)

struct CriticalGuard
{
    uint32_t prim;
    CriticalGuard(): prim(ENTER_CRITICAL()) {}
    ~CriticalGuard(){ EXIT_CRITICAL(prim); }
};