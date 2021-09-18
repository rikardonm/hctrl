#include <Arduino.h>

#include <SPI.h>

#include <gpio.hpp>

#include <nokia5110.hpp>
#include <console.hpp>


auto lcd = Nokia5110(SPI, Pin(PA4), Pin(PA6), Pin(PB0));
auto console = Console::Console(Serial1);



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


void setup()
{
    pinMode(PC13, OUTPUT);
    console.Init(115200);

    // lcd.Init();
}


void loop()
{
    auto mils = millis();
    console.EventLoopStep(mils);
    blink(mils);
}
