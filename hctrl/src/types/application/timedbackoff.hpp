#pragma once

#include <cstdint>
#include <cstddef>


/* Time-related activity helpers */
class TimedBackoff
{
public:
    TimedBackoff(uint32_t wait_time)
    : _last_ts(0), _backoff(wait_time), _last_elapsed(0)
    {
    }

    /**
     * @brief Check if protected action should be executed.
     *
     * @param now Current timestamp, in the same multiple/sub-multiple as the specified period.
     * @return true If action should be executed.
     * @return false If elapsed time is smaller than the specified period and action should not be executed.
     */
    bool Mark(uint32_t now)
    {
        if (not isExpired(now))
        {
            return false;
        }
        Remark(now);
        return true;
    }
    bool isExpired(uint32_t now)
    {
        auto elapsed = now - _last_ts;
        return elapsed >= _backoff;
    }
    uint32_t Elapsed()
    {
        return _last_elapsed;
    }
    void Remark(uint32_t now)
    {
        _last_ts = now;
    }
private:
    uint32_t _last_ts;
    uint32_t _backoff;
    uint32_t _last_elapsed;
};
