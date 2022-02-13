#pragma once

#include <cstdint>

#include <types/hal/gpio/gpio.hpp>
#include <types/application/timedbackoff.hpp>
#include <types/bal/button/button.hpp>


namespace RotaryEncoder
{
    struct Event
    {
        uint32_t elapsed;
        bool clock_wise;
        Event(const uint32_t elapsed, const bool clock_wise)
        : elapsed(elapsed), clock_wise(clock_wise)
        {}
        Event(const interfaces::Event::Message& msg)
        {
            Unpack(msg);
        }
        interfaces::Event::Message Pack(const interfaces::Event::ProducerID id);
    protected:
        const uint32_t _elapsed_mask = 0x7FFFFFFF;
        const uint32_t _clockws_mask = 0x80000000;

        Event Unpack(const interfaces::Event::Message& msg);
    };

    class RotaryEncoder : private types::GPIO::ICallback
    {
    public:
        struct ICallback
        {
            virtual void Rotation(const Event& message) = 0;
            virtual ~ICallback() = default;
            using ptr = ICallback*;
        };

        RotaryEncoder(types::GPIO::Pin& dt, types::GPIO::Pin& clk);
        void Init(bool invert = false);
        bool Configure(ICallback& user_hook);
        bool Configure(interfaces::Event::IConsumer& consumer, interfaces::Event::ProducerID id);

    private:
        types::GPIO::Pin& _dt;
        types::GPIO::Pin& _clk;

        TimedBackoff _dt_bakcoff;
        TimedBackoff _clk_backoff;

        bool _dt_last;
        bool _clk_last;

        void EncoderClock();
        void EncoderDirection();

        virtual void Interrupt(types::CallbackId_t id) override;

        ICallback::ptr _puser_callbacks = nullptr;
        interfaces::Event::IConsumer::ptr _puser_event = nullptr;
        interfaces::Event::ProducerID _event_id = 0x00;
    };

} /* namespace RotaryEncoder */
