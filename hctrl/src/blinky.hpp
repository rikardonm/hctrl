/*
 * blinky.hpp
 *
 *  Created on: Sep 25, 2021
 *      Author: ricardo
 */

#pragma once

#include <types/bal/led/led.hpp>
#include <types/application/periodicaction.hpp>


class Blinky : public PeriodicAction
{
public:
    Blinky(LED::LED& led, const uint32_t half_period)
    : _led(led), PeriodicAction(half_period)
    {}
    virtual ~Blinky() = default;
    virtual void Init() override
    {
    }
protected:
    virtual void Action() override
    {
        _led.Toggle();
    }

private:
    LED::LED& _led;
};
