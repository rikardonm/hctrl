#include <Arduino.h>

#include <SPI.h>

#include <gpio.hpp>

#include <nokia5110.hpp>
#include <console.hpp>


auto spi1 = SPIClass(PA7, PA6, PA5, PA4);

auto lcd = Nokia5110::Nokia5110(spi1, Pin(PA4), Pin(PA6), Pin(PB0));


Console::ReturnCode LcdWrite(Console::IOBuffer& input, Console::IOBuffer& output)
{
    return Console::ReturnCode::Success;
}


void blink(uint32_t mils)
{
    static bool state = false;
    static uint32_t last_ts = 0;
    const uint32_t half_period = 500;

    auto elapsed = mils - last_ts;
    if (elapsed >= half_period)
    {
        last_ts = mils;
        digitalWrite(PC13, state);
        state = !state;
    }
}


auto console = Console::Console(Serial1);


void setup()
{
    pinMode(PC13, OUTPUT);
    console.Init(115200);
    lcd.Init();
    Console::RegisterNewCommand({"lcdwrite", LcdWrite});
}

void loop()
{
    auto mils = millis();
    console.EventLoopStep(mils);
    blink(mils);
}
