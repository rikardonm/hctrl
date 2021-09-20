#pragma once

#include <cstdint>
#include <cstddef>


template<typename T, size_t SIZE>
constexpr size_t Length(const T (&array)[SIZE])
{
    return SIZE;
}

/* Time-related activity helpers */
class TimedBackoff
{
public:
    TimedBackoff(uint32_t wait_time)
    : _last_ts(0), _backoff(wait_time)
    {
    }
    bool Mark(uint32_t now)
    {
        auto elapsed = now - _last_ts;
        if (elapsed < _backoff)
        {
            return false;
        }
        _last_ts = now;
        return true;
    }
private:
    uint32_t _last_ts;
    uint32_t _backoff;
};


class PeriodicAction
{
public:
    PeriodicAction(const uint32_t period)
    : _backoff(period)
    {
    }
    virtual void Init();
    void EventLoopStep(uint32_t mils)
    {
        if (not _backoff.Mark(mils))
        {
            return;
        }
        Action();
    }
protected:
    virtual void Action();
    TimedBackoff _backoff;
};
