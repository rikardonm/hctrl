/*
 * blinky.hpp
 *
 *  Created on: Sep 25, 2021
 *      Author: ricardo
 */

#pragma once

#include <general/helpers.hpp>
#include <types/gpio/gpio.hpp>


class Blinky : public PeriodicAction
{
public:
    Blinky(types::GPIO::Pin& pin, const uint32_t half_period)
    : PeriodicAction(half_period), _state(false), _pin(pin)
    {
    }
    virtual ~Blinky() = default;
    virtual void Init() override
    {
        _pin.Configure(types::GPIO::Direction::OutputPushPull);
    }
protected:
    virtual void Action() override
    {
        _pin.Set(_state);
        _state = !_state;
    }

private:
    bool _state;
    types::GPIO::Pin& _pin;
};
