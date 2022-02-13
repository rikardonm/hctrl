/*
 * nokia5110.h
 *
 *  Created on: 22/07/2016
 *      Author: rikardonm
 */

#pragma once

#include <cstdint>
#include <memory>

#include <types/bal/nokia5110/nokia5110.hpp>


namespace dotmatrix
{
    class Text
    {
    public:
        Text(Nokia5110::Nokia5110& lcd)
            : _lcd(lcd)
        {}

        bool Init();

        void Char(char mychar);
        Text& operator<<(char mychar);

        void String(const char* src);
        Text& operator<<(const char* src);

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
        Text& operator<<(const int8_t number);
        Text& operator<<(const uint8_t number);
        Text& operator<<(const int16_t number);
        Text& operator<<(const uint16_t number);
        Text& operator<<(const int32_t number);
        Text& operator<<(const uint32_t number);
        Text& operator<<(const size_t number);

        void CharPosition(uint8_t x, uint8_t y);

        /**
         * @brief Clear LCD contents.
         *
         */
        void Clear();

        void Flush();

    private:
        /* Hardware handles */
        Nokia5110::Nokia5110& _lcd;
        /* State variables */
        bool _initd;
        bool _invert;
    };

    extern template void Text::Number(const int8_t number);
    extern template void Text::Number(const uint8_t number);
    extern template void Text::Number(const int16_t number);
    extern template void Text::Number(const uint16_t number);
    extern template void Text::Number(const int32_t number);
    extern template void Text::Number(const uint32_t number);
    extern template void Text::Number(const size_t number);

}
