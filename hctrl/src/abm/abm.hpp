/*
 * abm.hpp
 *
 *  Created on: Jan 29, 2022
 *      Author: ricardo
 */


#pragma once

#include <memory>
#include <cstdint>

#include <bal/user.hpp>

#include <types/abm/dotmatrix/text.hpp>
#include <system/console/console.hpp>


struct ABM : protected BSP
{
    using BSP::favorite_1;
    using BSP::favorite_2;
    using BSP::favorite_3;
    using BSP::selector;
    using BSP::selector_button;
    using BSP::red_led;

    static Console::Console console;
    static dotmatrix::Text text_out;

    static void Init();

    // // b9 - A
    // types::GPIO::Pin output_control_a;
    // // b15 - B
    // types::GPIO::Pin output_control_b;

}; /* struct ABM */
