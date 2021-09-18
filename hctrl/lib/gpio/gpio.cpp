#include "gpio.hpp"
#include "Arduino.h"


Pin::Pin(uint32_t num)
: pin_num(num)
{
}

void Pin::Configure(bool is_input)
{
	pinMode(pin_num, is_input ? INPUT : OUTPUT);
}

void Pin::Set(bool value)
{
	digitalWrite(pin_num, value);
}

bool Pin::Read(bool value)
{
	return digitalRead(pin_num);
}
