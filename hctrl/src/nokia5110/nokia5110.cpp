/*
 * nokia5110.c
 *
 *  Created on: 22/07/2016
 *      Author: rikardonm
 */

#include "nokia5110.hpp"
#include "nokia5110_typefaces.hpp"

#include <cstdlib>

// used for delay
#include <Arduino.h>


namespace Nokia5110
{

    namespace CommandCodes
    {
        namespace FunctionSet
        {
            const uint8_t Code = 0x20;
            const uint8_t ValueMask = 0x07;
        }
        namespace H0
        {
            namespace DisplayControl
            {
                const uint8_t Code = 0x08;
                const uint8_t ValueMask = 0x05;
            }
            namespace SetYAddress
            {
                const uint8_t Code = 0x40;
                const uint8_t ValueMask = 0x7;
            }
            namespace SetXAddress
            {
                const uint8_t Code = 0x80;
                const uint8_t ValueMask = 0x7F;
            }
        }
        namespace H1
        {
            namespace TemperatureControl
            {
                const uint8_t Code = 0x04;
                const uint8_t ValueMask = 0x03;
            }
            namespace BiasSystem
            {
                const uint8_t Code = 0x10;
                const uint8_t ValueMask = 0x07;
            }
            namespace Vop
            {
                const uint8_t Code = 0x80;
                const uint8_t ValueMask = 0x7F;
            }
        }
    }


    class TransactionManager
    {
    public:
        TransactionManager(
            types::GPIO::Pin& ssn,
            types::GPIO::Pin& data_commandn,
            bool is_data
        ) : _ssn(ssn),
            _data_commandn(data_commandn)
        {
            _ssn.Set(false);
            if (is_data)
            {
                Data();
            }
            else
            {
                Command();
            }
        }
        ~TransactionManager()
        {
            _ssn.Set(true);
        }
        void Data()
        {
            _data_commandn.Set(true);
        }
        void Command()
        {
            _data_commandn.Set(false);
        }
    private:
        types::GPIO::Pin& _ssn;
        types::GPIO::Pin& _data_commandn;
    };

    Nokia5110::Nokia5110(
        types::SPI::SPI& spi,
        types::GPIO::Pin& ssn,
        types::GPIO::Pin& data_commandn,
        types::GPIO::Pin& resetn
    ) : _spi(spi),
        _ssn(ssn),
        _resetn(resetn),
        _data_commandn(data_commandn),
        _initd(false),
        _runner(0),
        _invert(false)
    {
        _buffer.Zerofy();
    }

    bool Nokia5110::Init()
    {
        if (_initd)
        {
            return true;
        }

        _ssn.Configure(types::GPIO::Options(types::GPIO::Direction::Input));
        _ssn.Set(true);
        _data_commandn.Configure(types::GPIO::Options(types::GPIO::Direction::OutputPushPull));
        _data_commandn.Set(false);
        _resetn.Configure(types::GPIO::Options(types::GPIO::Direction::OutputPushPull));
        _resetn.Set(false);
        delayMicroseconds(500);
        _resetn.Set(true);

        /* Configure HW layer */
        auto options = types::SPI::Options();
        options.software_ssn = true;
        _spi.Init(options);

        /* Configure logical layer */
        /* Steps as defined in Section 13 Table 6 of PCD8544 LCD controller datasheet
            https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf
        */
       {
            auto mgr = TransactionManager(_ssn, _data_commandn, false);
            _spi.Transfer(FunctionSet(PowerDownControl::Active, AddressingMode::Horizontal, InstructionSetChoice::Extended));
            _spi.Transfer(Vop(0x40));
            _spi.Transfer(FunctionSet(PowerDownControl::Active, AddressingMode::Horizontal, InstructionSetChoice::Basic));
            _spi.Transfer(DisplayControl(DisplayModes::Normal));
            _spi.Transfer(BiasSystem(4));
        }
        CharPosition(0, 0);
        for(auto i = 0; i < Icons::Azevem::Width; ++i)
        {
            _PushToBuffer(Icons::Azevem::Data[i]);
        }
        /* add blank space */
        _PushToBuffer(0x00);
        String("Azevem!");
        CharPosition(0, 1);
        String("fuckOS");
        Flush(true);

        _initd = true;
        return true;
    }

    void Nokia5110::Char(char mychar)
    {
        if (mychar < Typeface::OriginalCopy::CharStartOffset or mychar >= Typeface::OriginalCopy::CharEndOffset)
        {
            mychar = '?';
        }

        mychar -= Typeface::OriginalCopy::CharStartOffset;

        for (int i = 0; i < Typeface::OriginalCopy::CharWidth; ++i)
        {
            _PushToBuffer(Typeface::OriginalCopy::CharTable[mychar][i]);
        }
        _PushToBuffer(0x00);
    }

    Nokia5110& Nokia5110::operator<<(char mychar)
    {
        Char(mychar);
        return *this;
    }

    Nokia5110& Nokia5110::operator<<(const char* src)
    {
        String(src);
        return *this;
    }

    void Nokia5110::String(const char* src)
    {
        while(*src)
        {
            Char(*src);
            ++src;
        }
    }

    void Nokia5110::PadLine()
    {
        auto cursor = 0;
        /* Find next line break */
        while(cursor < _runner)
        {
            cursor += Parameters::XPixels;
        }
        /* Pad to next line break */
        while(_runner < cursor)
        {
            _PushToBuffer(0x00);
        }
    }

    void Nokia5110::Invert(bool value)
    {
        _invert = value;
    }

    template<typename T>
    void Nokia5110::Number(const T number)
    {
        const auto length = (sizeof(T) * 8) + 1;
        char output[length];
        const auto dec_base = 10;
        itoa(number, output, dec_base);
        String(output);
    }

    template void Nokia5110::Number(const int8_t number);
    template void Nokia5110::Number(const uint8_t number);
    template void Nokia5110::Number(const int16_t number);
    template void Nokia5110::Number(const uint16_t number);
    template void Nokia5110::Number(const int32_t number);
    template void Nokia5110::Number(const uint32_t number);
    template void Nokia5110::Number(const size_t number);

    Nokia5110& Nokia5110::operator<<(const int8_t number)
    {
        Number(number);
        return *this;
    }

    Nokia5110& Nokia5110::operator<<(const uint8_t number)
    {
        Number(number);
        return *this;
    }

    Nokia5110& Nokia5110::operator<<(const int16_t number)
    {
        Number(number);
        return *this;
    }

    Nokia5110& Nokia5110::operator<<(const uint16_t number)
    {
        Number(number);
        return *this;
    }

    Nokia5110& Nokia5110::operator<<(const int32_t number)
    {
        Number(number);
        return *this;
    }

    Nokia5110& Nokia5110::operator<<(const uint32_t number)
    {
        Number(number);
        return *this;
    }

    Nokia5110& Nokia5110::operator<<(const size_t number)
    {
        Number(number);
        return *this;
    }

    void Nokia5110::Position(uint8_t x, uint8_t y)
    {
        if (x > Parameters::XPixels)
        {
            x = Parameters::XPixels;
        }
        if (y > Parameters::YBanks)
        {
            y = Parameters::YBanks;
        }
        _runner = (Parameters::XPixels * y) + x;
    }

    void Nokia5110::CharPosition(uint8_t x, uint8_t y)
    {
        Position(x * (Typeface::OriginalCopy::CharWidth + 1), y);
    }

    void Nokia5110::Clear()
    {
        _buffer.Zerofy();
        _runner = 0x00;
    }

    void Nokia5110::Flush(bool pad)
    {
        if (pad)
        {
            uint8_t pad_char = _invert ? 0xFF : 0x00;
            memset(&_buffer.data[_runner], pad_char, sizeof(_buffer) - _runner);
            _runner = sizeof(_buffer);
        }
        auto mgr = TransactionManager(_ssn, _data_commandn, false);
        _spi.Transfer(SetXAddress(0));
        _spi.Transfer(SetYAddress(0));
        mgr.Data();
        _spi.Transfer(_buffer);
        _runner = 0;
    }

    void Nokia5110::AdjustForTemperature(uint8_t temp_c)
    {
        /* Although the LCD driver has temperature compensation, it this feature chooses the automatic compensation
         * curve, and is not directly dependent on the current temperature. */
        return;
    }

    void Nokia5110::AdjustForPowerMode(uint8_t power_mode)
    {
        return;
    }

    void Nokia5110::AdjustForSupplyVoltage(uint8_t voltage)
    {
        return;
    }

    constexpr uint8_t Nokia5110::FunctionSet(PowerDownControl power_down, AddressingMode entry_mode, InstructionSetChoice instruction_set)
    {
        return CommandCodes::FunctionSet::Code | (CommandCodes::FunctionSet::ValueMask & (power_down | entry_mode | instruction_set));
    }

    constexpr uint8_t Nokia5110::DisplayControl(DisplayModes mode)
    {
        return CommandCodes::H0::DisplayControl::Code | (CommandCodes::H0::DisplayControl::ValueMask & mode);
    }

    constexpr uint8_t Nokia5110::SetYAddress(uint8_t value)
    {
        const auto y_bank_offset = Parameters::YBanks - 1;
        if (value > y_bank_offset)
        {
            value = y_bank_offset;
        }
        return CommandCodes::H0::SetYAddress::Code | (CommandCodes::H0::SetYAddress::ValueMask & value);
    }

    constexpr uint8_t Nokia5110::SetXAddress(uint8_t value)
    {
        const auto x_pixel_offset = Parameters::XPixels - 1;
        if (value > x_pixel_offset)
        {
            value = x_pixel_offset;
        }
        return CommandCodes::H0::SetXAddress::Code | (CommandCodes::H0::SetXAddress::ValueMask & value);
    }

    constexpr uint8_t Nokia5110::TemperatureControl(uint8_t value)
    {
        return CommandCodes::H1::TemperatureControl::Code | (CommandCodes::H1::TemperatureControl::ValueMask & value);
    }

    constexpr uint8_t Nokia5110::BiasSystem(uint8_t value)
    {
        return CommandCodes::H1::BiasSystem::Code | (CommandCodes::H1::BiasSystem::ValueMask & value);
    }

    constexpr uint8_t Nokia5110::Vop(uint8_t value)
    {
        return CommandCodes::H1::Vop::Code | (CommandCodes::H1::Vop::ValueMask & value);
    }

    void Nokia5110::_PushToBuffer(const uint8_t value)
    {
        if (_runner < sizeof(_buffer))
        {
            if (_invert)
            {
                _buffer.data[_runner] = ~value;
            }
            else
            {
                _buffer.data[_runner] = value;
            }
            ++_runner;
        }
    }

} /* namespace Nokia5110 */
