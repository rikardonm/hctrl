/*
 * user.cpp
 *
 *  Created on: Oct 3, 2021
 *      Author: ricardo
 */

#include <general.hpp>
#include "user.hpp"
#include <platform/platform.hpp>
#include <arduino.h>

namespace BSP
{
    enum class pins : uint32_t
    {
        spi_mosi = PA7,
        spi_miso = PA6,
        spi_clk = PA5,
        // lcd_ssn = PB10,
        // lcd_data_command = PB1,
        // lcd_reset = PB11,
        // favorite_1 = PB7,
        // favorite_2 = PB5,
        // favorite_3 = PA8,
        output_a = PB9,
        output_b = PB15,
        encoder_sw = PB3,
        encoder_dt = PB4,
        encoder_clk = PA15,
        led = PC13,
    };

    /* Construct static objects */
    Nokia5110::Nokia5110 lcd = Nokia5110::Nokia5110(
        platform::spi_1,
        lcd_ssn,
        lcd_data_command,
        lcd_reset
    );

    RotaryEncoder::RotaryEncoder selector = RotaryEncoder::RotaryEncoder(
        BSP::encoder_sw,
        BSP::encoder_dt,
        BSP::encoder_clk
    );

    void Init()
    {
        // favorites
        // b7
        auto opt = types::GPIO::Options(types::GPIO::Direction::Input, types::GPIO::PullResistor::Up);
        favorite1.Configure(opt);
        // GPIO::Pin favorite_pin_1 = GPIO::GetPin(pins::favorite_1);
        // b5
        favorite2.Configure(opt);
        // GPIO::Pin favorite_pin_2 = GPIO::GetPin(pins::favorite_2);
        // a8 // closer to connector
        favorite3.Configure(opt);
        // GPIO::Pin favorite_pin_3 = GPIO::GetPin(pins::favorite_3);

        lcd.Init();
        selector.Init(false);


        // b9 - A
        // GPIO::Pin output_control_a = GPIO::GetPin(pins::output_a);
        // b15 - B
        // GPIO::Pin output_control_b = GPIO::GetPin(pins::output_b);

    }

} /* namespace BSP */
