/*
 * user.hpp
 *
 *  Created on: Oct 3, 2021
 *      Author: ricardo
 */


#pragma once

#include <memory>
#include <cstdint>

#include <types/bal/nokia5110/nokia5110.hpp>
#include <types/bal/rotary_encoder/rotary_encoder.hpp>
#include <types/bal/button/button.hpp>
#include <types/bal/led/led.hpp>

#include <platform/arduino/platform.hpp>


struct BSP
{
    /* Perhaps add the Schematic reference? */
    static Button::Button favorite_1;
    static Button::Button favorite_2;
    static Button::Button favorite_3;                           // Closer to connector
    static LED::LED red_led;

    /* Expose platform pins */
    static const types::GPIO::Pin* serial_rx;
    static const types::GPIO::Pin* serial_tx;

    static Nokia5110::Nokia5110 lcd;

    // user LCD exposes a "framebuffer" interface??
    // while the abm shim exposes write functions

    static Button::Button selector_button;
    static RotaryEncoder::RotaryEncoder selector;

    static void Init();

    // // b9 - A
    // types::GPIO::Pin output_control_a;
    // // b15 - B
    // types::GPIO::Pin output_control_b;

}; /* struct BSP */
