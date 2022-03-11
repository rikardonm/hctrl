#pragma once

#include <cstdint>

#include <Arduino.h>

#include <types/types.hpp>
#include <types/interfaces/string.hpp>
#include <types/storage/circular_buffer.hpp>
#include <types/system/console/options.hpp>


namespace Console
{
    /**
     * @brief
     *
     */
    class IOBuffer :  public interfaces::StringStream
    {
    private:
        using _Base = CircularBuffer<char, Options::BufferLength, Options::CleanBuffer>;
        _Base _buffer;

        virtual void _InsertString(const char* const string) override
        {
            auto len = strlen(string);
            if (_buffer.Free() >= len)
            {
                for(auto i = 0; i < len; ++i)
                {
                    _buffer.Push(string[i]);
                }
            }
        }
        virtual void _InsertChar(const char chr)
        {
            if (_buffer.Free() > 1)
            {
                _buffer.Push(chr);
            }
        }
    public:
        using Cursor = _Base::CircularBufferCursor;
        _Base& GetBuffer()
        {
            return _buffer;
        }
    };

    /**
     * @brief Check if supplied cstring is equal to data contained in the buffer between start and end cursors.
     *
     * @param start Inclusive cursor.
     * @param end Not inclusive cursor.
     * @param test_str
     * @return true
     * @return false
     */
    bool IsStringEqual(
        IOBuffer::Cursor& start,
        const IOBuffer::Cursor& end,
        const char* test_str
    );

    /**
     * @brief
     *
     * @tparam FCNPTR
     */
    template<typename FCNPTR>
    struct Command
    {
        using FunctionPtr = FCNPTR;
        const char* name;
        FunctionPtr run;
    };

    /**
     * @brief
     *
     */
    class Console
    {
    public:
        /**
         * @brief Construct a new Console object
         *
         * @param serial_block
         * @param verbose_codes
         */
        Console(HardwareSerial& serial_block, bool verbose_codes = false);

        ~Console();
        Console(const Console& other) = delete;
        Console& operator=(const Console& other) = delete;

        /**
         * @brief
         *
         * @param baud_rate
         */
        void Init(uint32_t baud_rate);

        /**
         * @brief
         *
         */
        void DeInit();

        /**
         * @brief
         *
         * @param mils
         */
        void EventLoopStep(uint32_t mils);

        IOBuffer& StdOut();

    private:
        HardwareSerial& _serial;
        IOBuffer _input_buffer;
        IOBuffer _output_buffer;
        bool _verbose_codes;

        /**
         * @brief
         *
         * @param code
         */
        void PrintControlCode(char code);

        /**
         * @brief
         *
         */
        void PrintCloseInput();

        /**
         * @brief
         *
         */
        void FlushOutput();

        /**
         * @brief
         *
         */
        void ParseInputExecuteCommand();

        /* Built-in function commands */
        const char* ConsoleOptionsCommand = "cli";

        /**
         * @brief
         *
         * @param input
         * @param output
         * @return ReturnCode
         */
        types::ReturnCode Help(IOBuffer& input, IOBuffer& output);

        /**
         * @brief
         *
         * @param input
         * @param output
         * @return ReturnCode
         */
        types::ReturnCode VerboseCodes(IOBuffer& input, IOBuffer& output);

        const Command<types::ReturnCode (Console::*)(IOBuffer& input, IOBuffer& output)> _options[2] =
        {
            {"help", &Console::Help},
            {"ccodes", &Console::VerboseCodes},
        };
    };

    using StandaloneCommand = Command<types::ReturnCode (*)(IOBuffer& input, IOBuffer& output)>;
    extern StandaloneCommand builtin_commands[];

    /**
     * @brief Register a new command to the console.
     *
     * @param newcommand New command object to be registered.
     * @return true If new command could be successfully added.
     * @return false If command inclusion failed.
     */
    bool RegisterNewCommand(StandaloneCommand newcommand);

} /* namespace Console */
