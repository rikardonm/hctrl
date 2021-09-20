/*
 * nokia5110.h
 *
 *  Created on: 22/07/2016
 *      Author: rikardonm
 */


#pragma once

#include <cstdint>
#include <SPI.h>
#include <gpio/gpio.hpp>

namespace Nokia5110
{
    enum DisplayModes : uint8_t
    {
        DisplayBlank = 0x00,
        Normal = 0x04,
        AllSegmentsOn = 0x01,
        InverseVideo = 0x05,
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
        Nokia5110(SPIClass& spi, Pin& ssn, Pin& data_command, Pin& reset);
        void Init(void);
        void Char(char mychar);
        void String(const char* src);

        template<typename T>
        void Number(const T number);

        /**
         * @brief Set the cursor position in the LCD
         *
         * @param x Coordinate from screen's left border, in pixels
         * @param y Coordinate from screen's top border, in rows
         */
        void Position(uint8_t x, uint8_t y);

        void CharPosition(uint8_t x, uint8_t y);

        /**
         * @brief Clear LCD contents.
         *
         */
        void Clear();

        void AdjustForTemperature(uint8_t temp_c);
        void AdjustForPowerMode(uint8_t power_mode);
        void AdjustForSupplyVoltage(uint8_t voltage);

    private:
        SPIClass& _spi;
        Pin& _ssn;
        Pin& _resetn;
        Pin& _data_commandn;
        bool _initd;

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

        void _Char(uint8_t mychar);
        void _String(const char* src);
        void _Clear();
    };

    void reverse(char s[]);
    void myitoa(unsigned int n, char s[], bool leading_zeros = false);

    extern template void Nokia5110::Number(const int8_t number);
    extern template void Nokia5110::Number(const uint8_t number);
    extern template void Nokia5110::Number(const int16_t number);
    extern template void Nokia5110::Number(const uint16_t number);
    extern template void Nokia5110::Number(const int32_t number);
    extern template void Nokia5110::Number(const uint32_t number);
    extern template void Nokia5110::Number(const size_t number);

}
