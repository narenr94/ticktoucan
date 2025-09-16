#include "ticktoucan_platform.h"

#include <thread>
#include <atomic>
#include <chrono>

static std::atomic<bool> tick_thread_running{false};
static std::thread tick_thread;


uint32_t platform_enter_critical()
{
    return 0;
}


void platform_exit_critical(uint32_t primask)
{}

void platform_setup_tick_timer(uint32_t tick_ms, void (*tickCb)())
{
    // Stop any previous thread
    tick_thread_running = false;
    if (tick_thread.joinable()) {
        tick_thread.join();
    }

    tick_thread_running = true;
    tick_thread = std::thread([tick_ms, tickCb]() {
        while (tick_thread_running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(tick_ms));
            if (tickCb) tickCb();
        }
    });
}

void platform_cleanup_tick_timer()
{
    tick_thread_running = false;
    if (tick_thread.joinable()) {
        tick_thread.join();
    }
}