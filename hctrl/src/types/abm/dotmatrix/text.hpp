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

        /**
         * @brief Initialize block and its dependencies.
         *
         * @return true If successful.
         * @return false If any failure ocurred.
         */
        bool Init();

        /**
         * @brief Set cursor write position in underlying display object.
         *
         * @param x Position in the X-axis, in pixels.
         * @param y Position in the Y-axis, in pixels.
         */
        void CharPosition(uint8_t x, uint8_t y);

        /**
         * @brief Clear LCD contents.
         *
         */
        void Clear();

        /**
         * @brief Flushes the buffer in the underlying display object.
         *
         */
        void Flush();

        /**
         * @brief Insert a 8x8 bit Azevem icon into the current buffer location.
         * 
         */
        void InsertAzevemIcon();

    private:
        /* Hardware handles */
        Nokia5110::Nokia5110& _lcd;
        /* State variables */
        bool _initd;

        virtual void _InsertString(const char* const string) override;
        virtual void _InsertChar(const char chr) override;
    };
}
