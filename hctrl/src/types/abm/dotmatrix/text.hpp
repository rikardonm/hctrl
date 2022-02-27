/*
 * nokia5110.h
 *
 *  Created on: 22/07/2016
 *      Author: rikardonm
 */

#pragma once

#include <cstdint>

#include <types/interfaces/string.hpp>

#include <types/bal/nokia5110/nokia5110.hpp>


namespace dotmatrix
{
    class Text : public interfaces::StringStream
    {
    public:
        Text(Nokia5110::Nokia5110& lcd)
            : _lcd(lcd)
        {}

        bool Init();

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

        virtual void _InsertString(const char* const string) override;
        virtual void _InsertChar(const char chr) override;
    };
}
