// #include <variant_BLUEPILL_F103C6.h>
// #include <stm32f103x6.h>

// #include <Arduino.h>

#include <cstdint>

#include <types/storage/circular_buffer.hpp>
#include <types/storage/fifo.hpp>

#include <system/log/log.hpp>

#include <fwork/base.hpp>

#include <bal/user.hpp>
#include <abm/abm.hpp>

#include "blinky.hpp"

#include <application/application.hpp>


auto lcd_backoff = TimedBackoff(50);
const auto splash_screen_delay = 1000;

auto blinky = Blinky(ABM::red_led, 500);


// Console::ReturnCode SetFavorite(Console::IOBuffer& input, Console::IOBuffer& output)
// {
//     input.Discard(sizeof("setf"), false);
//     auto [success, value] = input.Pop();
//     if (not success)
//     {
//         return Console::ReturnCode::Unknown;
//     }
//     switch(value)
//     {
//         case '0':
//             app.state.favorites[0] = 144;
//             break;
//         case '1':
//             app.state.favorites[1] = 185;
//             break;
//         case '2':
//             app.state.favorites[2] = 210;
//             break;
//         default:
//             return Console::ReturnCode::Unknown;
//     }
//     return Console::ReturnCode::Success;
// }

// Console::StandaloneCommand SetFavoriteCommand = {"setf", SetFavorite};



void setup()
{
    ABM::Init();
    Log::console.SetLevel(Log::Level::Trace);
    blinky.Init();

    // Console::RegisterNewCommand(SetFavoriteCommand);

    app.Init();

    /* let the splash screens sink in... */
    // delay(splash_screen_delay);
    // ABM::lcd.Clear();
}

void loop()
{
    const auto mils = millis();
    blinky.EventLoopStep(mils);
    ABM::console.EventLoopStep(mils);
    // auto [success, value] = app.events.Pop();
    // if (success)
    // {
    //     current_state.ProcessMessage(mils, value);
    // }
    // if (lcd_backoff.Mark(mils))
    // {
    //     current_state.UpdateOutput(mils, OutputTypes::Nokia5110, 0);
    // }
    // current_state.UpdateTick(mils);
}
