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

    auto& lcd_ssn = platform::pin_30;               // PB10
    auto& lcd_data_command = platform::pin_0xC9;    // PB1
    auto& lcd_reset = platform::pin_31;             // PB11

    auto& encoder_sw = platform::pin_6;             // PB3
    auto& encoder_dt = platform::pin_5;             // PB4
    auto& encoder_clk = platform::pin_7;            // PA15


    enum class pins : uint32_t
    {
        // spi_mosi = PA7,
        // spi_miso = PA6,
        // spi_clk = PA5,
        // lcd_ssn = PB10,
        // lcd_data_command = PB1,
        // lcd_reset = PB11,
        // favorite_1 = PB7,
        // favorite_2 = PB5,
        // favorite_3 = PA8,
        output_a = PB9,
        output_b = PB15,
        // encoder_sw = PB3,
        // encoder_dt = PB4,
        // encoder_clk = PA15,
        // led = PC13,
    };

    /***************************************************************************
     *  Construct static objects
     **************************************************************************/
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

    // TODO: reference something from platform!
    Console:: Console console = Console::Console(Serial1);

    void Init()
    {
        // Console debug + human interaction
        console.Init(ConsoleBaudRate);
        // favorites
        // b7
        auto opt = types::GPIO::Options(types::GPIO::Direction::Input, types::GPIO::PullResistor::Up);
        favorite1->Configure(opt);
        // b5
        favorite2->Configure(opt);
        // a8 // closer to connector
        favorite3->Configure(opt);

        lcd.Init();
        selector.Init(false);

        // b9 - A
        // GPIO::Pin output_control_a = GPIO::GetPin(pins::output_a);
        // b15 - B
        // GPIO::Pin output_control_b = GPIO::GetPin(pins::output_b);

    }

} /* namespace BSP */
