/*
 * button.hpp
 *
 *  Created on: Sep 26, 2021
 *      Author: ricardo
 */


#pragma once

#include <types/gpio/gpio.hpp>
#include <general/helpers.hpp>

namespace Button
{
    class Button
    {
    public:
    private:
        types::GPIO::Pin& _pin;
    };
} /* namespace Button */
