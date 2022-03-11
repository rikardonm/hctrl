
#pragma once
#include <cstring>
#include <cstdlib>

#include <general/ascii.hpp>


namespace interfaces
{

    class StringStream
    {
    public:

        void InsertNewLine()
        {
            *this
                << static_cast<char>(ASCII::ControlCodes::LineFeed)
                << static_cast<char>(ASCII::ControlCodes::CarriageReturn);
        }

        /**
         * @brief
         *
         * @param string String to insert into buffer.
         * @param insert_newline If true, appends the new line marker (CR/CRLF/LF) into the buffer after the string.
         */
        void InsertString(const char* string, bool insert_newline = false)
        {
            _InsertString(string);
            if (insert_newline)
            {
                InsertNewLine();
            }
        }

        StringStream& operator<<(const char value)
        {
            _InsertChar(value);
            return *this;
        }
        StringStream& operator<<(const char* value)
        {
            _InsertString(value);
            return *this;
        }

        StringStream& operator<<(const int8_t number)
        {
            Number(number);
            return *this;
        }

        StringStream& operator<<(const uint8_t number)
        {
            Number(number);
            return *this;
        }

        StringStream& operator<<(const int16_t number)
        {
            Number(number);
            return *this;
        }

        StringStream& operator<<(const uint16_t number)
        {
            Number(number);
            return *this;
        }

        StringStream& operator<<(const int32_t number)
        {
            Number(number);
            return *this;
        }

        StringStream& operator<<(const uint32_t number)
        {
            Number(number);
            return *this;
        }

        StringStream& operator<<(const size_t number)
        {
            Number(number);
            return *this;
        }

        virtual ~StringStream() = default;
        using ptr = StringStream*;

    private:
        virtual void _InsertString(const char* const string) = 0;
        virtual void _InsertChar(const char chr) = 0;

        template<typename T>
        void Number(const T number)
        {
            const auto length = (sizeof(T) * 8) + 1;
            char output[length];
            const auto dec_base = 10;
            itoa(number, output, dec_base);
            *this << output;
        }
    };

} /* namespace interfaces */
