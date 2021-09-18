/*
 * nokia5110.h
 *
 *  Created on: 22/07/2016
 *      Author: rikardonm
 */


#pragma once

#include <cstdint>
#include <SPI.h>
#include "gpio.hpp"

/*
 *
 * PINOUT and selectors
 *
 * for TIVA 123 launchpad
 *
 * 		 _____________
 * ----X-|--		--|-o----
 * ----X-|--		--|-X----
 * ----o-|--		--|-X----
 * 		 |	DISPLAY	  |
 * ----X-|--		--|-o----
 * ----X-|--		--|-X----
 * ----o-|--		--|-X----
 * 		 |			  |
 * 		 |			  |
 * 		 |____________|
 *
 *
 * Upper Left:	DIN
 * 		up:		PA5		<--- THIS
 * 		dw:		PB7
 *
 * Upper Right:	CLK
 * 		up:		PB4
 * 		dw:		PA2		<--- THIS
 *
 * Lower Left:	SLV
 * 		up:		PA4		<--- THIS
 * 		dw:		PE5
 *
 * Lower Right:	D/C
 * 		up:		PB6
 * 		dw:		PA3		<--- THIS
 *
 * all in SSI0 module
 *
 *
 *
 ******************************************
 *
 *
 *
 * for TIVA 1294 launchpad
 * in BOOSTERPACK 1
 * 		 _____________
 * ----o-|--		--|-X----
 * ----X-|--		--|-X----
 * ----X-|--		--|-o----
 * 		 |	DISPLAY	  |
 * ----X-|--		--|-o----
 * ----X-|--		--|-X----
 * ----o-|--		--|-X----
 * 		 |			  |
 * 		 |			  |
 * 		 |____________|
 *
 *  * Upper Left:	DIN
 * 		up:		PC7
 * 		dw:		PD1		<--- THIS
 *
 * Upper Right:	CLK
 * 		up:		PD3		<--- THIS
 * 		dw:		PP2
 *
 * Lower Left:	SLV
 * 		up:		PN2		<--- THIS
 * 		dw:		PE5
 *
 * Lower Right:	D/C
 * 		up:		PD0
 * 		dw:		PN3		<--- THIS
 *
 * all in SSI2 module
 *
 *
 */



enum DisplayModes : uint8_t
{
	DisplayBlank = 0x00,
	NormalMode = 0x04,
	AllSegmentsOn = 0x01,
	InverseVideoMode = 0x05,
};

enum PowerDownControl : uint8_t
{
	Active = 0x00,
	PowerDown = 0x40,
};

enum AddressingMode : uint8_t
{
	Horizontal = 0x00,
	Vertical = 0x02,
};

enum InstructionSetChoice : uint8_t
{
	Basic = 0x00,
	Extended = 0x01,
};



class Nokia5110
{
public:
	Nokia5110(SPIClass spi, Pin ssn, Pin data_command, Pin reset);
	void Init(void);
	void Char(char mychar);
	void String(const char* src);
	void Pos(uint8_t x, uint8_t y);

	void AdjustForTemperature(uint8_t temp_c);
	void AdjustForPowerMode(uint8_t power_mode);
	void AdjustForSupplyVoltage(uint8_t voltage);

private:
	SPIClass spi;
	Pin ssn;
	Pin resetn;
	Pin data_commandn;
	bool initd;

	/* Commands */
	constexpr uint8_t FunctionSet(PowerDownControl power_down, AddressingMode entry_mode, InstructionSetChoice instruction_set);
	/* Commands H = 0 */
	constexpr uint8_t DisplayControl(DisplayModes mode);
	constexpr uint8_t SetYAddress(uint8_t value);
	constexpr uint8_t SetXAddress(uint8_t value);
	/* Commands H = 1 */
	constexpr uint8_t TemperatureControl(uint8_t value);
	constexpr uint8_t BiasSystem(uint8_t value);
	constexpr uint8_t Vop(uint8_t value);

	void Cmd(uint8_t data);
	void Raw(uint8_t data);
	void Raw(const uint8_t* data, size_t len, bool is_command);
};

void reverse(char s[]);
void myitoa(unsigned int n, char s[]);
