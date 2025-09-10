#include "ticktoucan.h"


static TickToucan::TickToucan& instance()
{
    static TickToucan inst;
    return inst;
}

void TickToucan::tickISR()
{
    ++currentTick_;
    // Check each task slot
    for (std::size_t i = 0; i < MAX_TASKS; ++i)
    {
        Task& t = tasks_[i];
        if (!t.active) continue;
        if (currentTick_ >= t.nextTick)
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

void TickToucan::dispatch()
{
    for (std::size_t i = 0; i < MAX_TASKS; ++i)
    {
        Task& t = tasks_[i];
        if (t.ready)
        {
            t.ready = false;            // clear the flag
            t.cb(t.ctx);                // execute user callback
        }
    }
}

Handle TickToucan::scheduleAt(TickType absoluteTick, Callback cb, void* ctx = nullptr)
{
    return addTask(absoluteTick, 0, cb, ctx);
}

Handle TickToucan::scheduleEvery(TickType interval, Callback cb, void* ctx = nullptr)
{
    return addTask(currentTick_ + interval, interval, cb, ctx);
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

bool TickToucan::valid(Handle h) const 
{
    return h.idx >= 0 && h.idx < static_cast<int>(MAX_TASKS);
}

Handle TickToucan::addTask(TickType next, TickType interval, Callback cb, void* ctx)
{
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
