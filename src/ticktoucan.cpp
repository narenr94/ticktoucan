//ticktoucan.cpp

#include "ticktoucan.h"
#include "ticktoucan_platform.h"

TickToucan::Task::Task()
{
    active = false;
    nextTick = 0;
    interval = 0;
    cb = nullptr;
    ctx = nullptr;
    ready = false;
}


TickToucan& TickToucan::instance()
{
  static TickToucan inst;
  return inst;
}

void TickToucan::tickISR()
{
    incrementTick();
    scheduleTask();    
}

void TickToucan::dispatch()
{
    for (std::size_t i = 0; i < MAX_TASKS; ++i)
    {
        Task& t = tasks_[i];
        if (t.ready)
        {
            uint32_t prim = ENTER_CRITICAL();
            t.ready = false;            // clear the flag
            EXIT_CRITICAL(prim);
            t.cb(t.ctx);                // execute user callback
        }
    }
}

TickToucan::Handle TickToucan::scheduleAt(TickType absoluteTick, Callback cb, void* ctx)
{
    return addTask(absoluteTick, 0, cb, ctx);
}

TickToucan::Handle TickToucan::scheduleEvery(TickType interval, Callback cb, void* ctx, TickType t_offset)
{
    return addTask(now() + t_offset, interval, cb, ctx);
}

void TickToucan::cancel(Handle h)
{
    if (valid(h))
    {
        tasks_[h.idx].active = false;
        tasks_[h.idx].ready  = false;
    }
}

TickType TickToucan::now() const
{
    return currentTick_;
}

TickToucan::TickToucan() : currentTick_(0)
{
    for (auto& t : tasks_)
    {
        t.active = false;
        t.ready  = false;
    }
}

TickToucan::~TickToucan()
{
    platform_cleanup_tick_timer();
}

bool TickToucan::valid(Handle h) const 
{
    return h.idx >= 0 && h.idx < static_cast<int>(MAX_TASKS);
}

TickToucan::Handle TickToucan::addTask(TickType next, TickType interval, Callback cb, void* ctx)
{
    CriticalGuard guard;

    for (std::size_t i = 0; i < MAX_TASKS; ++i)
    {
        Task& t = tasks_[i];
        if (!t.active && !t.ready)
        {
            t.active   = true;
            t.ready    = false;
            t.nextTick = next;
            t.interval = interval;
            t.cb       = cb;
            t.ctx      = ctx;
            return Handle{static_cast<int>(i)};
        }
    }
    return Handle{-1};  // no free slot
}

void TickToucan::incrementTick()
{
    ++currentTick_;
}

void TickToucan::scheduleTask()
{
    // Check each task slot
    for (std::size_t i = 0; i < MAX_TASKS; ++i)
    {
        Task& t = tasks_[i];
        if (!t.active) continue;
        if (now() >= t.nextTick)
        {
            t.ready = true;
            if (t.interval > 0)
            {
                // schedule next fire
                t.nextTick += t.interval;
            }
            else
            {
                // one-shot: deactivate
                t.active = false;
            }
        }
    }
}

void TickToucan::init(uint32_t tick_ms)
{
    platform_setup_tick_timer(tick_ms, &TickToucan::tickTrampoline);

    m_tick_ms = tick_ms;
}

void TickToucan::tickTrampoline()
{
    TickToucan::instance().tickISR();
}
