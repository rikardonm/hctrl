#include <rotary_encoder/rotary_encoder.hpp>
#include <rotary_encoder/options.hpp>

#include <Arduino.h>

#include <general/helpers.hpp>


namespace RotaryEncoder
{
    RotaryEncoder::RotaryEncoder(Pin& sw, Pin& dt, Pin& clk, uint32_t& clicked, int32_t& rotations)
    : _sw(sw), _dt(dt), _clk(clk),
    _sw_backoff(Options::ClickBackoff), _dt_bakcoff(Options::RotateBackoff), _clk_backoff(Options::RotateBackoff),
    _dt_last(false), _clk_last(false),
    _clicked(clicked), _rotations(rotations)
    {
    }

    void RotaryEncoder::Init()
    {
        _sw.Configure(true, true);
        _clk.Configure(true, false);
        _dt.Configure(true, false);

        attachInterrupt(digitalPinToInterrupt(_sw.Number()), EncoderClick, RISING);
        attachInterrupt(digitalPinToInterrupt(_clk.Number()), EncoderClock, CHANGE);
        attachInterrupt(digitalPinToInterrupt(_dt.Number()), EncoderDirection, CHANGE);
    }

    void EncoderClick(void)
    {
        if (not encoder._sw_backoff.Mark(millis()))
        {
            return;
        }
        ++encoder._clicked;
    }

    void EncoderClock(void)
    {
        auto now = millis();
        if (not encoder._clk_backoff.Mark(millis()))
        {
            return;
        }

        /* criss-cross buffering */
        encoder._dt_last = encoder._dt.Read();

        bool decrement = encoder._clk_last ^ encoder._dt_last;

        if (decrement)
        {
            --encoder._rotations;
        }
        else
        {
            ++encoder._rotations;
        }
    }

    void EncoderDirection(void)
    {
        if (not encoder._dt_bakcoff.Mark(millis()))
        {
            return;
        }
        /* criss-cross buffering */
        encoder._clk_last = encoder._clk.Read();
    }
}
