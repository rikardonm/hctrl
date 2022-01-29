/*
 * user.hpp
 *
 *  Created on: Oct 3, 2021
 *      Author: ricardo
 */


#pragma once

#include <memory>
#include <cstdint>

#include <Arduino.h>

#include <HardwareSerial.h>
#include <SPI.h>

#include <nokia5110/nokia5110.hpp>
#include <rotary_encoder/rotary_encoder.hpp>
#include <console/console.hpp>

#include <platform/platform.hpp>


namespace BSP
{



    /* well, here, I think I solved it.
    the thing is, we can't really use using, or use references.
    we technically are exposing (already) the logical abstraction of what is on our board.
    it's not like we have a random pin just floating around.
    And if we do, making a platform pin a pointer makes it most terribly clear!

    as in the case for the status LED.... well, it is an LED, not really a GPIO.
    sure, the LED driver can be built on top of a "DigitalIO"
    if one was to use the pin with the adc or with the dac, one can define an analog in or analog out!

    and then the GPIO::Pin abstraction, what does it actually hide?
    hummm, the platform pin technically has all functions enabled: digital set/read, alternate function, slew-rate, etc.
    it would, technically, mimmic exactly the GPIO port/pin representation.

    */

    /* Expose platform pins */
    const auto favorite1 = &platform::pin_2;              // PB7
    const auto favorite2 = &platform::pin_4;              // PB5
    const auto favorite3 = &platform::pin_12;             // PA8

    const auto status_led = &platform::pin_17;            // PC13

    extern Nokia5110::Nokia5110 lcd;

    extern RotaryEncoder::RotaryEncoder selector;

    const auto ConsoleBaudRate = 115200;
    extern Console::Console console;

    void Init();

    // // b9 - A
    // types::GPIO::Pin output_control_a;
    // // b15 - B
    // types::GPIO::Pin output_control_b;

}
