
#pragma once

#include <types/types.hpp>
#include <types/hal/gpio/gpio.hpp>

namespace LED
{

    class LED
    {
    public:
        LED(types::GPIO::Pin& pin, bool active_high = false)
            : _pin(pin), _active_high(active_high)
        {}

        void Init()
        {
            _pin.Configure(types::GPIO::Direction::OutputPushPull);
        }

        void On()
        {
            _state = true;
            _pin.Set(_active_high);
        }
        void Off()
        {
            _state = false;
            _pin.Set(not _active_high);
        }
        void Toggle()
        {
            if (_state)
            {
                Off();
            }
            else
            {
                On();
            }
        }

    private:
        types::GPIO::Pin& _pin;
        bool _active_high;
        bool _state;
    };

} /* namespace Button */
