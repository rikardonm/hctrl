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

    void Text::CharPosition(uint8_t x, uint8_t y)
    {
        _lcd.Position(x, y);
    }

    void Text::Clear()
    {
        _lcd.Clear();
    }

    void Text::Flush()
    {
        _lcd.Flush();
    }

    void Text::InsertAzevemIcon()
    {
        for(auto& c : Icons::Azevem::Data)
        {
            _lcd.PushToBuffer(c);
        }
    }

} /* namespace Nokia5110 */
