/*
 * nokia5110.c
 *
 *  Created on: 22/07/2016
 *      Author: rikardonm
 */


#include <cstdlib>

// used for delay
#include <Arduino.h>

#include "text.hpp"
#include "text_typefaces.hpp"

namespace dotmatrix
{

    bool Text::Init()
    {
        if (_initd)
        {
            return true;
        }
        if (not _lcd.Init())
        {
            return false;
        }
        /* add blank space */
        _lcd.PushToBuffer(0x00);
        String("Azevem!");
        CharPosition(0, 1);
        String("fuckOS");
        _lcd.Flush(true);

        CharPosition(0, 0);
        for(auto i = 0; i < Icons::Azevem::Width; ++i)
        {
            _lcd.PushToBuffer(Icons::Azevem::Data[i]);
        }

        _initd = true;
        return true;
    }

    void Text::Char(char mychar)
    {

        if (mychar < Typeface::OriginalCopy::CharStartOffset or mychar >= Typeface::OriginalCopy::CharEndOffset)
        {
            mychar = '?';
        }

        mychar -= Typeface::OriginalCopy::CharStartOffset;

        for (int i = 0; i < Typeface::OriginalCopy::CharWidth; ++i)
        {
            _lcd.PushToBuffer(Typeface::OriginalCopy::CharTable[mychar][i]);
        }
        _lcd.PushToBuffer(0x00);
    }

    Text& Text::operator<<(char mychar)
    {
        Char(mychar);
        return *this;
    }

    Text& Text::operator<<(const char* src)
    {
        String(src);
        return *this;
    }

    void Text::String(const char* src)
    {
        while(*src)
        {
            Char(*src);
            ++src;
        }
    }

    void Text::PadLine()
    {
        _lcd.PadLine();
    }

    void Text::Invert(bool value)
    {
        _invert = value;
    }

    template<typename T>
    void Text::Number(const T number)
    {
        const auto length = (sizeof(T) * 8) + 1;
        char output[length];
        const auto dec_base = 10;
        itoa(number, output, dec_base);
        String(output);
    }

    template void Text::Number(const int8_t number);
    template void Text::Number(const uint8_t number);
    template void Text::Number(const int16_t number);
    template void Text::Number(const uint16_t number);
    template void Text::Number(const int32_t number);
    template void Text::Number(const uint32_t number);
    template void Text::Number(const size_t number);

    Text& Text::operator<<(const int8_t number)
    {
        Number(number);
        return *this;
    }

    Text& Text::operator<<(const uint8_t number)
    {
        Number(number);
        return *this;
    }

    Text& Text::operator<<(const int16_t number)
    {
        Number(number);
        return *this;
    }

    Text& Text::operator<<(const uint16_t number)
    {
        Number(number);
        return *this;
    }

    Text& Text::operator<<(const int32_t number)
    {
        Number(number);
        return *this;
    }

    Text& Text::operator<<(const uint32_t number)
    {
        Number(number);
        return *this;
    }

    Text& Text::operator<<(const size_t number)
    {
        Number(number);
        return *this;
    }

    void Text::CharPosition(uint8_t x, uint8_t y)
    {
        // Hideous for this high-up layer to access hard-coded constants
        // But oh well, later on we clean it up.
        _lcd.Position(x * (Typeface::OriginalCopy::CharWidth + 1), y);
    }

    void Text::Clear()
    {
        _lcd.Clear();
    }

    void Text::Flush()
    {
        _lcd.Flush(false);
    }

} /* namespace Nokia5110 */
