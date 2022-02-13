/*
 * user.cpp
 *
 *  Created on: Oct 3, 2021
 *      Author: ricardo
 */

#include <general.hpp>
#include "user.hpp"
#include <platform/arduino/platform.hpp>
#include <arduino.h>


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
Button::Button BSP::favorite_1(platform::pin_2);                 // PB7
Button::Button BSP::favorite_2(platform::pin_4);                 // PB5
Button::Button BSP::favorite_3(platform::pin_12);                // PA8
LED::LED BSP::red_led(platform::pin_17);                         // PC13

types::GPIO::Pin* serial_rx = &platform::pin_10;                 // PA10
types::GPIO::Pin* serial_tx = &platform::pin_11;                 // PA9

Nokia5110::Nokia5110 BSP::lcd(platform::spi_1, lcd_ssn, lcd_data_command, lcd_reset);

Button::Button BSP::selector_button(encoder_sw);
RotaryEncoder::RotaryEncoder BSP::selector(encoder_dt, encoder_clk);


void BSP::Init()
{
    // favorites
    // b7
    favorite_1.Init(false, true);
    favorite_2.Init(false, true);
    favorite_3.Init(false, true);
    red_led.Init();

    lcd.Init();
    selector_button.Init(false, true);
    selector.Init(false);

    // b9 - A
    // GPIO::Pin output_control_a = GPIO::GetPin(pins::output_a);
    // b15 - B
    // GPIO::Pin output_control_b = GPIO::GetPin(pins::output_b);
}
