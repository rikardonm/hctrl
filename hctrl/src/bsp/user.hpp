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

#include <platform/platform.hpp>


namespace BSP
{


    auto& lcd_ssn = platform::pin_30;               // PB10
    auto& lcd_data_command = platform::pin_0xC9;    // PB1
    auto& lcd_reset = platform::pin_31;             // PB11
    extern Nokia5110::Nokia5110 lcd;

    /* Expose platform pins */
    auto& favorite1 = platform::pin_2;              // PB7
    auto& favorite2 = platform::pin_4;              // PB5
    auto& favorite3 = platform::pin_12;             // PA8

    auto& status_led = platform::pin_17;            // PC13

    auto& encoder_sw = platform::pin_6;             // PB3
    auto& encoder_dt = platform::pin_5;             // PB4
    auto& encoder_clk = platform::pin_7;            // PA15
    extern RotaryEncoder::RotaryEncoder selector;

    void Init();

    // // b9 - A
    // types::GPIO::Pin output_control_a;
    // // b15 - B
    // types::GPIO::Pin output_control_b;

}
