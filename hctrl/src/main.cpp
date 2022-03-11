// #include <variant_BLUEPILL_F103C6.h>
// #include <stm32f103x6.h>

// #include <Arduino.h>

#include <cstdint>

#include <types/types.hpp>
#include <types/storage/circular_buffer.hpp>
#include <types/storage/fifo.hpp>

#include <types/system/log/log.hpp>

#include <fwork/base.hpp>

#include <bal/user.hpp>
#include <abm/abm.hpp>

#include "blinky.hpp"

#include <application/application.hpp>

// Testing
#include <types/bal/nokia5110/test.hpp>


auto lcd_backoff = TimedBackoff(50);

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



types::ReturnCode Test(Console::IOBuffer& input, Console::IOBuffer& output)
{
    // todo: parse arguments and determine which test to run
    // input.Discard(sizeof("setf"), false);
    // auto [success, value] = input.Pop();
    // if (not success)
    // {
    //     return types::ReturnCode::InvalidArguments;
    // }
    BSP::lcd.Init();
    delay(10);
    uint8_t val = 0x00;
    for (auto i = 0; i < 8; ++i)
    {
        Nokia5110::Test::FillPattern(BSP::lcd, val);
        delay(1000);
        val |= 0x01;
        val << 1;
    }
    return types::ReturnCode::Success;
}

Console::StandaloneCommand TestHardware = {"test", Test};




void setup()
{
    ABM::Init();
    Log::console.SetLevel(Log::Level::Trace);
    blinky.Init();

    // Console::RegisterNewCommand(SetFavoriteCommand);
    Console::RegisterNewCommand(TestHardware);

    app.Init();

    BSP::lcd.Clear();
    auto& x = ABM::text_out;
    x.CharPosition(0, 0);
    x << "hello fucker!";
    x.Flush();
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
    if (lcd_backoff.Mark(mils))
    {
        current_state.UpdateOutput(mils, OutputTypes::Nokia5110, 0);
    }
    current_state.UpdateTick(mils);
}
