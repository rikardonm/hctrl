/*
 * nokia5110.h
 *
 *  Created on: 22/07/2016
 *      Author: rikardonm
 */


#pragma once

#include <cstdint>
#include <memory>

#include <types/storage/array.hpp>
#include <types/hal/gpio/gpio.hpp>
#include <types/hal/spi/spi.hpp>


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
        const uint32_t YPixelsPerBank = 8;
        const uint32_t YBanks = YPixels / YPixelsPerBank;
    }

    class Nokia5110
    {
    public:
        using Buffer_t = types::Array<uint8_t, Parameters::XPixels * Parameters::YBanks>;

        Nokia5110(
            types::SPI::SPI& spi,
            types::GPIO::Pin& ssn,
            types::GPIO::Pin& data_commandn,
            types::GPIO::Pin& reset
        );

        bool Init();

        /**
         * @brief Set the cursor position in the LCD.
         *
         * @param x Coordinate from screen's left border, in pixels.
         * @param y Coordinate from screen's top border, in pixels.
         */
        void Position(uint8_t x, uint8_t y);

        /**
         * @brief Clear LCD contents.
         *
         */
        void Clear();

        /**
         * @brief Transfer image buffer to device.
         *
         */
        void Flush();

        void AdjustForTemperature(uint8_t temp_c);
        void AdjustForPowerMode(uint8_t power_mode);
        void AdjustForSupplyVoltage(uint8_t voltage);

        /**
         * @brief Push a vertical cursor to display, up to 8 bits high.
         *
         * @param value Cursor data to be pushed.
         */
        void PushToBuffer(const uint8_t value);

        /**
         * @brief Copy buffer into image buffer.
         *
         * @param buf Source buffer to copy data from.
         * @param index Starting index at destination buffer.
         */
        void CopyBuffer(const types::iArray<const uint8_t>& buf, uint32_t index = 0);

        constexpr size_t GetBufferLength()
        {
            return _buffer.size;
        }

    private:
        /* Hardware handles */
        types::SPI::SPI& _spi;
        types::GPIO::Pin& _ssn;
        types::GPIO::Pin& _resetn;
        types::GPIO::Pin& _data_commandn;
        /* State variables */
        bool _initd;
        uint8_t _xpos;
        uint8_t _ypos;
        Buffer_t _buffer;

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
    };

} /* namespace Nokia5110 */
