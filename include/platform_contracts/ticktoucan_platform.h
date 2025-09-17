//ticktoucan_platform.h

#pragma once

#include <cstdint>


/**
 * @brief Disables interrupts and returns the current interrupt mask for later restoration.
 * @return The current interrupt mask.
 */
uint32_t platform_enter_critical();

/**
 * @brief Restores the interrupt mask, re-enabling interrupts as appropriate.
 * @param primask The interrupt mask to restore.
 */
void platform_exit_critical(uint32_t primask);

/**
 * @brief Installs a millisecond tick timer that invokes the specified callback at the given interval.
 * @param tick_ms The tick interval in milliseconds.
 * @param tickCb Pointer to the callback function to be called on each tick.
 * @note This function may be called multiple times to reconfigure or reset the tick interval and callback. Any previously running tick timer should be stopped and replaced.
 */
void platform_setup_tick_timer(uint32_t tick_ms, void (*tickCb)());

/**
 * @brief Stops and cleans up the tick timer.
 */
void platform_cleanup_tick_timer();

#define ENTER_CRITICAL()  platform_enter_critical()
#define EXIT_CRITICAL(p)  platform_exit_critical(p)

struct CriticalGuard
{
    uint32_t prim;
    CriticalGuard(): prim(ENTER_CRITICAL()) {}
    ~CriticalGuard(){ EXIT_CRITICAL(prim); }
};