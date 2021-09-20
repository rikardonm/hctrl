#pragma once

#include <cstdint>

#include <gpio/gpio.hpp>
#include <general/helpers.hpp>

namespace RotaryEncoder
{

    void EncoderClick(void);
    void EncoderClock(void);
    void EncoderDirection(void);

    class RotaryEncoder
    {
    public:
        RotaryEncoder(Pin& sw, Pin& dt, Pin& clk, uint32_t& clicked, int32_t& rotations);
        void Init();
    private:
        Pin& _sw;
        Pin& _dt;
        Pin& _clk;
        TimedBackoff _sw_backoff;
        TimedBackoff _dt_bakcoff;
        TimedBackoff _clk_backoff;
        bool _dt_last;
        bool _clk_last;

        uint32_t& _clicked;
        int32_t& _rotations;

        friend void EncoderClick(void);
        friend void EncoderClock(void);
        friend void EncoderDirection(void);
    };

    /* User must define object in their source files */
    extern RotaryEncoder encoder;
}
