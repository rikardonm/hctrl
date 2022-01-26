/*
 * nokia5110.h
 *
 *  Created on: 22/07/2016
 *      Author: rikardonm
 */


#pragma once

#include <cstdint>
#include <memory>

#include <types/gpio/gpio.hpp>

#include <types/spi/spi.hpp>


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

    namespace Parameters
    {
        const uint32_t XPixels = 84;
        const uint32_t YPixels = 48;
        const uint32_t YBanks = 6;
    }

    class Nokia5110
    {
    public:
        Nokia5110(
            types::SPI::SPI& spi,
            types::GPIO::Pin& ssn,
            types::GPIO::Pin& data_commandn,
            types::GPIO::Pin& reset
        );

        bool Init();

        void Char(char mychar);
        Nokia5110& operator<<(char mychar);

        void String(const char* src);
        Nokia5110& operator<<(const char* src);

        /**
         * @brief Add empty spaces to end of line in LCD.
         *
         * @pre
         * @post
         */
        void PadLine();

        /**
         * @brief Invert pixel values when writing to LCD.
         *
         * @pre
         * @post
         * @param value Enable or disables pixel color inversion.
         */
        void Invert(bool value);

        template<typename T>
        void Number(const T number);
        Nokia5110& operator<<(const int8_t number);
        Nokia5110& operator<<(const uint8_t number);
        Nokia5110& operator<<(const int16_t number);
        Nokia5110& operator<<(const uint16_t number);
        Nokia5110& operator<<(const int32_t number);
        Nokia5110& operator<<(const uint32_t number);
        Nokia5110& operator<<(const size_t number);

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

        void Flush(bool pad);

        void AdjustForTemperature(uint8_t temp_c);
        void AdjustForPowerMode(uint8_t power_mode);
        void AdjustForSupplyVoltage(uint8_t voltage);

    private:
        /* Hardware handles */
        types::SPI::SPI& _spi;
        types::GPIO::Pin& _ssn;
        types::GPIO::Pin& _resetn;
        types::GPIO::Pin& _data_commandn;
        /* State variables */
        bool _initd;
        size_t _runner;
        types::Array<uint8_t, Parameters::XPixels * Parameters::YBanks> _buffer;
        bool _invert;

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

        void _PushToBuffer(const uint8_t value);
    };

    extern template void Nokia5110::Number(const int8_t number);
    extern template void Nokia5110::Number(const uint8_t number);
    extern template void Nokia5110::Number(const int16_t number);
    extern template void Nokia5110::Number(const uint16_t number);
    extern template void Nokia5110::Number(const int32_t number);
    extern template void Nokia5110::Number(const uint32_t number);
    extern template void Nokia5110::Number(const size_t number);

}
