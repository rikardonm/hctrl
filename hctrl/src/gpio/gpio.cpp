#include "gpio.hpp"
#include "Arduino.h"


Pin::Pin(uint32_t num)
: pin_num(num)
{
}

void Pin::Configure(bool is_input, bool use_pullup)
{
	pinMode(pin_num, is_input ? use_pullup ? INPUT_PULLUP : INPUT : OUTPUT);
}

void Pin::Set(bool value)
{
	digitalWrite(pin_num, value);
}

bool Pin::Read()
{
	return digitalRead(pin_num);
}

uint32_t Pin::Number()
{
	return pin_num;
}
