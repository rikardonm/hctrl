#include <Arduino.h>

#include <SPI.h>

#include <gpio/gpio.hpp>

#include <nokia5110/nokia5110.hpp>
#include <console/console.hpp>
#include <rotary_encoder/rotary_encoder.hpp>

#include <general/helpers.hpp>






Pin porta[] =  {
    {PA0},
    {PA1},
    {PA2},
    {PA3},
    {PA4},
    {PA5},
    {PA6},
    {PA7},
    {PA8},
    {PA9},
    {PA10},
    {PA11},
    {PA12},
    {PA13},
    {PA14},
    {PA15},
};

Pin portb[] =  {
    {PB0},
    {PB1},
    {PB2},
    {PB3},
    {PB4},
    {PB5},
    {PB6},
    {PB7},
    {PB8},
    {PB9},
    {PB10},
    {PB11},
    {PB12},
    {PB13},
    {PB14},
    {PB15},
};

Pin portc[] =  {
    {PC13},
    {PC14},
    {PC15},
};










auto spi1 = SPIClass(PA7, PA6, PA5, PA4);


class Blinky : public PeriodicAction
{
public:
    Blinky(Pin& pin, const uint32_t half_period)
    : PeriodicAction(half_period), _state(false), _pin(pin)
    {
    }
    virtual void Init() override
    {
        _pin.Configure(false);
    }
protected:
    virtual void Action() override
    {
        digitalWrite(_pin.Number(), _state);
        _state = !_state;
    }

private:
    bool _state;
    Pin& _pin;
};



struct
{
    int32_t menu_item_index = 0;
    uint32_t click_count = 0;
} appState;




class RefreshLCD : public PeriodicAction
{
public:
    RefreshLCD(Nokia5110::Nokia5110& lcd)
    : PeriodicAction(150), _lcd(lcd)
    {
    }
    virtual void Init() override
    {
        _lcd.Clear();
        _lcd.Position(0, 0);
        _lcd.String("Item: ");
        _lcd.Position(0, 1);
        _lcd.String("Click: ");
    }
protected:
    virtual void Action() override
    {
        _lcd.CharPosition(6, 0);
        _lcd.String("   ");
        _lcd.CharPosition(6, 0);
        _lcd.Number(appState.menu_item_index);
        _lcd.CharPosition(7, 1);
        _lcd.String("   ");
        _lcd.CharPosition(7, 1);
        _lcd.Number(appState.click_count);
    }
private:
    Nokia5110::Nokia5110& _lcd;
};


auto lcd = Nokia5110::Nokia5110(spi1, porta[4], porta[6], portb[0]);
auto console = Console::Console(Serial1);
auto blinky = Blinky(portc[0], 500);
auto lcd_output = RefreshLCD(lcd);


/* Declare the single instance */
namespace RotaryEncoder
{
    RotaryEncoder encoder = RotaryEncoder(
        portb[15],
        portb[13],
        portb[14],
        appState.click_count,
        appState.menu_item_index
    );
}

auto& encoder = RotaryEncoder::encoder;


void setup()
{
    console.Init(115200);
    lcd.Init();
    /* let the splash screens sink in... */
    delay(1000);
    blinky.Init();
    lcd_output.Init();
    encoder.Init();
}

void loop()
{
    auto mils = millis();
    console.EventLoopStep(mils);
    blinky.EventLoopStep(mils);
    lcd_output.EventLoopStep(mils);
}
