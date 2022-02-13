#pragma once

#include <cstdint>
#include <cstddef>

#include <types/application/timedbackoff.hpp>


class PeriodicAction
{
public:
    PeriodicAction(const uint32_t period)
    : _backoff(period)
    {
    }
    virtual ~PeriodicAction() = default;
    virtual void Init()
    {}
    void EventLoopStep(const uint32_t mils)
    {
        if (not _backoff.Mark(mils))
        {
            return;
        }
        Action();
    }
protected:
    virtual void Action() = 0;
    TimedBackoff _backoff;
};
