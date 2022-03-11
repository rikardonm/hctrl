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
        _lcd.Position(0, 0);
        _lcd.PushToBuffer(0x00);
        *this << "Azevem!";
        CharPosition(0, 1);
        _lcd.PushToBuffer(0x00);
        *this << "fuckOS";
        _lcd.Flush(true);

        CharPosition(0, 0);
        for(auto i = 0; i < Icons::Azevem::Width; ++i)
        {
            _lcd.PushToBuffer(Icons::Azevem::Data[i]);
        }
        _lcd.Flush(false);

        _initd = true;
        return true;
    }

    void Text::_InsertString(const char* const string)
    {
        for(auto i = 0; string[i] != '\0'; ++i)
        {
            _InsertChar(string[i]);
        }
    }

    void Text::_InsertChar(const char mychar)
    {
        auto chr =
            (mychar < Typeface::OriginalCopy::CharStartOffset or mychar >= Typeface::OriginalCopy::CharEndOffset) ?
            '?' :
            mychar - Typeface::OriginalCopy::CharStartOffset;

        for (int i = 0; i < Typeface::OriginalCopy::CharWidth; ++i)
        {
            _lcd.PushToBuffer(Typeface::OriginalCopy::CharTable[chr][i]);
        }
        _lcd.PushToBuffer(0x00);
    }

    void Text::PadLine()
    {
        _lcd.PadLine();
    }

    void Text::Invert(bool value)
    {
        _invert = value;
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
