#pragma once

#include <cstdint>

#include <types/gpio/gpio.hpp>
#include <general/helpers.hpp>

namespace RotaryEncoder
{
    class Callback
    {
    public:
        virtual void Click(uint32_t elapsed) = 0;
        virtual void Rotation(uint32_t elapsed, bool clock_wise) = 0;
        using ptr = Callback*;
    };

    class RotaryEncoder
    {
    public:
        RotaryEncoder(
            types::GPIO::Pin& sw,
            types::GPIO::Pin& dt,
            types::GPIO::Pin& clk
        );
        void Init(bool invert = false);
        bool Configure(Callback& user_hook);

    private:
        types::GPIO::Pin& _sw;
        types::GPIO::Pin& _dt;
        types::GPIO::Pin& _clk;
        TimedBackoff _sw_backoff;
        TimedBackoff _dt_bakcoff;
        TimedBackoff _clk_backoff;
        bool _dt_last;
        bool _clk_last;

        void EncoderSwitch();
        void EncoderClock();
        void EncoderDirection();

        class CallbackHook : public types::GPIO::Callback
        {
        public:
            virtual void Interrupt(types::CallbackId_t id) override;
            RotaryEncoder& object;
            CallbackHook(RotaryEncoder& object) : object(object) {}
        };

        CallbackHook _callbacks = {*this};
        Callback::ptr _user_callbacks = nullptr;
    };

} /* namespace RotaryEncoder */
