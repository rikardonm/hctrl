/*
 * nokia5110.c
 *
 *  Created on: 22/07/2016
 *      Author: rikardonm
 */

#include "nokia5110.hpp"
#include "nokia5110_typefaces.hpp"


namespace Nokia5110
{
    enum ModuleBaseCommandCodes : uint8_t
    {
        FunctionSet = 0x20,
    };

    enum ModuleH0CommandCodes : uint8_t
    {
        DisplayControl = 0x08,
        SetYAdddress = 0x40,
        SetXAddress = 0x80,
    };

    enum ModuleH1CommandCodes : uint8_t
    {
        TemperatureControl = 0x04,
        BiasSystem = 0x10,
        Vop = 0x80,
    };

    class TransactionManager
    {
    public:
        TransactionManager(Pin ssn, Pin data_commandn, bool is_data)
        : _ssn(ssn), _data_commandn(data_commandn)
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
        Pin& _ssn;
        Pin& _data_commandn;
    };

    Nokia5110::Nokia5110(SPIClass& spi, Pin ssn, Pin data_commandn, Pin reset)
    : _spi(spi), _ssn(ssn), _resetn(reset), _data_commandn(data_commandn), _initd(false)
    {
    }

    void Nokia5110::Init()
    {
        if (_initd)
        {
            return;
        }

        /* Configure HW layer */
        _spi.begin();

        _ssn.Configure(false);
        _ssn.Set(true);
        _data_commandn.Configure(false);
        _data_commandn.Set(false);
        _resetn.Configure(false);
        _resetn.Set(false);
        delayMicroseconds(500);
        _resetn.Set(true);

        /* Configure logical layer */
        /* Steps as defined in Section 13, Table 6 of PCD8544 LCD controller datasheet
            https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf
        */

        auto mgr = TransactionManager(_ssn, _data_commandn, false);

        _spi.transfer(FunctionSet(PowerDownControl::Active, AddressingMode::Horizontal, InstructionSetChoice::Extended));
        _spi.transfer(Vop(0x40));
        _spi.transfer(FunctionSet(PowerDownControl::Active, AddressingMode::Horizontal, InstructionSetChoice::Basic));
        _spi.transfer(DisplayControl(DisplayModes::Normal));
        _spi.transfer(BiasSystem(4));

        mgr.Data();
        /* Clean the LCD  - 8 bits at a time */
        for (auto i = 0; i < (48 * 84 / 8); ++i)
        {
            _spi.transfer(0x00);
        }

        mgr.Command();
        _spi.transfer(SetXAddress(0));
        _spi.transfer(SetYAddress(0));

        mgr.Data();
        for(auto i = 0; i < Icons::Azevem::Width; ++i)
        {
            _spi.transfer(Icons::Azevem::Data[i]);
        }
        /* add blank space */
        _spi.transfer(0x00);
        _String("Azevem!");

        mgr.Command();
        _spi.transfer(SetXAddress(0));
        _spi.transfer(SetYAddress(1));

        mgr.Data();
        _String("fuckOS");

        _initd = true;
    }

    void Nokia5110::Char(char mychar)
    {
        auto mgr = TransactionManager(_ssn, _data_commandn, true);
        _Char(mychar);
    }

    void Nokia5110::String(const char* src)
    {
        auto mgr = TransactionManager(_ssn, _data_commandn, true);
        _String(src);
    }

    void Nokia5110::Pos(uint8_t x, uint8_t y)
    {
        auto mgr = TransactionManager(_ssn, _data_commandn, false);
        _spi.transfer(SetXAddress(x));
        _spi.transfer(SetYAddress(y));
    }

    constexpr uint8_t Nokia5110::FunctionSet(PowerDownControl power_down, AddressingMode entry_mode, InstructionSetChoice instruction_set)
    {
        return ModuleBaseCommandCodes::FunctionSet | power_down | entry_mode | instruction_set;
    }

    constexpr uint8_t Nokia5110::DisplayControl(DisplayModes mode)
    {
        return ModuleH0CommandCodes::DisplayControl | mode;
    }

    constexpr uint8_t Nokia5110::SetYAddress(uint8_t value)
    {
        if (value > 5)
        {
            value = 5;
        }
        return ModuleH0CommandCodes::SetYAdddress | value;
    }

    constexpr uint8_t Nokia5110::SetXAddress(uint8_t value)
    {
        if (value > 83)
        {
            value = 83;
        }
        return ModuleH0CommandCodes::SetXAddress | value;
    }

    constexpr uint8_t Nokia5110::TemperatureControl(uint8_t value)
    {
        return ModuleH1CommandCodes::TemperatureControl | (value & 0x03);
    }

    constexpr uint8_t Nokia5110::BiasSystem(uint8_t value)
    {
        return ModuleH1CommandCodes::BiasSystem | (value & 0x07);
    }

    constexpr uint8_t Nokia5110::Vop(uint8_t value)
    {
        return ModuleH1CommandCodes::Vop | (value & 0x7F);
    }

    void Nokia5110::_Char(uint8_t mychar)
    {
        if (mychar < Typeface::OriginalCopy::CharStartOffset or mychar >= Typeface::OriginalCopy::CharEndOffset)
        {
            mychar = '?';
        }

        mychar -= Typeface::OriginalCopy::CharStartOffset;

        for (int i = 0; i < Typeface::OriginalCopy::CharWidth; ++i)
        {
            _spi.transfer(Typeface::OriginalCopy::CharTable[mychar][i]);
        }

        /* add blank space */
        _spi.transfer(0x00);
    }

    void Nokia5110::_String(const char* src)
    {
        while(*src)
        {
            _Char(*src);
            ++src;
        }
    }


    /* reverse:  reverse string s in place */
    void reverse(char s[])
    {
        int c, i, j;

        for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
        {
            c = s[i];
            s[i] = s[j];
            s[j] = c;
        }
    }

    /* itoa:  convert n to characters in s */
    void myitoa(unsigned int n, char s[])
    {
        int i;
        i = 0;
        do
        /* generate digits in reverse order */
        {
            s[i++] = n % 10 + '0';   /* get next digit */
        } while ((n /= 10) > 0);     /* delete it */

        s[i] = '\0';
        reverse(s);
    }

}
