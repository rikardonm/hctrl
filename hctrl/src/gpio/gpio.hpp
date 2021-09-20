#pragma once

#include <cstdint>

#include <Arduino.h>

class Pin
{
public:
	Pin(uint32_t num);
	void Configure(bool is_input, bool use_pullup = false);
	void Set(bool value);
	bool Read();
	uint32_t Number();
private:
	uint32_t pin_num;
	bool is_input;
};
