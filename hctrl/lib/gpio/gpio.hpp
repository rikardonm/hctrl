
#pragma once

#include <cstdint>


class Pin
{
public:
	Pin(uint32_t num);
	void Configure(bool is_input);
	void Set(bool value);
	bool Read(bool value);
	uint32_t Number();
private:
	uint32_t pin_num;
	bool is_input;
};
