#include <types/bal/rotary_encoder/rotary_encoder.hpp>
#include <types/bal/rotary_encoder/options.hpp>
#include <types/types.hpp>

#include <Arduino.h>


namespace RotaryEncoder
{
    interfaces::Event::Message Event::Pack(const interfaces::Event::ProducerID id)
    {
        interfaces::Event::Message msg = {
            .producer_id = id,
            .data = (elapsed & _elapsed_mask)
        };
        if (clock_wise)
        {
            msg.data |= _clockws_mask;
        }
        else
        {
            msg.data &= ~_clockws_mask;
        }
        return msg;
    }

    Event Event::Unpack(const interfaces::Event::Message& msg)
    {
        elapsed =  msg.data & _elapsed_mask;
        clock_wise = msg.data & _clockws_mask;
    }

    enum IOIds : types::CallbackId_t
    {
        ClockId,
        DirectionId,
    };

    RotaryEncoder::RotaryEncoder(types::GPIO::Pin& dt, types::GPIO::Pin& clk)
    :
        _dt(dt),
        _clk(clk),
        _dt_bakcoff(Options::RotateBackoff),
        _clk_backoff(Options::RotateBackoff),
        _dt_last(false),
        _clk_last(false)
    {
    }

    void RotaryEncoder::Init(bool invert)
    {
        namespace IO = types::GPIO;
        auto io_opt = IO::Options(IO::Direction::Input, IO::PullResistor::Up);
        auto isr_opt = IO::IsrOptions(IO::InterruptMode::Rising, 0x00);
        isr_opt.pcallback_object = static_cast<types::GPIO::ICallback*>(this);
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

    bool RotaryEncoder::Configure(ICallback& user_hook)
    {
        _puser_callbacks = &user_hook;
        return true;
    }

    bool RotaryEncoder::Configure(interfaces::Event::IConsumer& consumer, interfaces::Event::ProducerID id)
    {
        _puser_event = &consumer;
        _event_id = id;
        return true;
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
        Event msg(_clk_backoff.Elapsed(),not decrement);

        if (_puser_callbacks)
        {
            _puser_callbacks->Rotation(msg);
        }
        else if (_puser_event)
        {
            _puser_event->StoreIncomingMessage(msg.Pack(_event_id));
        }
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

    void RotaryEncoder::Interrupt(types::CallbackId_t id)
    {
        switch(id)
        {
            case ClockId:
                EncoderClock();
                break;
            case DirectionId:
                EncoderDirection();
                break;
            default:
                break;
        };
    }

} /* namespace RotaryEncoder */
