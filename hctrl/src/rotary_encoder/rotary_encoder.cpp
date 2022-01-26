#include <rotary_encoder/rotary_encoder.hpp>
#include <rotary_encoder/options.hpp>

#include <Arduino.h>

#include <general/helpers.hpp>


namespace RotaryEncoder
{

    enum : types::CallbackId_t
    {
        SwitchId,
        ClockId,
        DirectionId,
    };

    RotaryEncoder::RotaryEncoder(types::GPIO::Pin& sw, types::GPIO::Pin& dt, types::GPIO::Pin& clk)
    : _sw(sw), _dt(dt), _clk(clk),
    _sw_backoff(Options::ClickBackoff), _dt_bakcoff(Options::RotateBackoff), _clk_backoff(Options::RotateBackoff),
    _dt_last(false), _clk_last(false)
    {
    }

    void RotaryEncoder::Init(bool invert)
    {
        namespace IO = types::GPIO;
        auto io_opt = IO::Options(IO::Direction::Input, IO::PullResistor::Up);
        auto isr_opt = IO::IsrOptions(IO::InterruptMode::Rising, SwitchId, _callbacks);
        _sw.Configure(io_opt, isr_opt);
        if (invert)
        {
            isr_opt.id = ClockId;
            _clk.Configure(io_opt, isr_opt);
            isr_opt.id = DirectionId;
            _dt.Configure(io_opt, isr_opt);
        }
        else
        {
            isr_opt.id = DirectionId;
            _clk.Configure(io_opt, isr_opt);
            isr_opt.id = ClockId;
            _dt.Configure(io_opt, isr_opt);
        }
    }

    bool RotaryEncoder::Configure(Callback& user_hook)
    {
        _user_callbacks = &user_hook;
        return true;
    }

    void RotaryEncoder::EncoderSwitch()
    {
        if (not _sw_backoff.Mark(millis()))
        {
            return;
        }
        if (not _user_callbacks)
        {
            return;
        }
        _user_callbacks->Click(_sw_backoff.Elapsed());
    }

    void RotaryEncoder::EncoderClock()
    {
        auto now = millis();
        if (not _clk_backoff.Mark(millis()))
        {
            return;
        }

        /* criss-cross buffering */
        _dt_last = _dt.Read();

        bool decrement = _clk_last ^ _dt_last;

        if (not _user_callbacks)
        {
            return;
        }

        _user_callbacks->Rotation(_clk_backoff.Elapsed(), not decrement);
    }

    void RotaryEncoder::EncoderDirection()
    {
        if (not _dt_bakcoff.Mark(millis()))
        {
            return;
        }
        /* criss-cross buffering */
        _clk_last = _clk.Read();
    }

    void RotaryEncoder::CallbackHook::Interrupt(types::CallbackId_t id)
    {
        switch(id)
        {
            case SwitchId:
                object.EncoderSwitch();
                break;
            case ClockId:
                object.EncoderClock();
                break;
            case DirectionId:
                object.EncoderDirection();
                break;
            default:
                break;
        };
    }

} /* namespace RotaryEncoder */
