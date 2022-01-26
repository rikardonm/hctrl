#pragma once

#include <cstdint>

#include <Arduino.h>

#include <general/circular_buffer.hpp>

#include <console/options.hpp>


namespace Console
{
    /**
     * @brief
     *
     */
    class IOBuffer : public CircularBuffer<char, Options::BufferLength, Options::CleanBuffer>
    {
    private:
        using _Base = CircularBuffer<char, Options::BufferLength, Options::CleanBuffer>;
    public:

        /**
         * @brief
         *
         */
        void InsertNewLine();

        /**
         * @brief
         *
         * @param string
         * @param insert_newline
         */
        void InsertString(const char* string, bool insert_newline = false);
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
        IOBuffer::CircularBufferCursor start,
        const IOBuffer::CircularBufferCursor& end,
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
    enum class ReturnCode
    {
        Success = 0x00,
        Unknown = 0x01,
        Timeout = 0x02,
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
        ReturnCode Help(IOBuffer& input, IOBuffer& output);

        /**
         * @brief
         *
         * @param input
         * @param output
         * @return ReturnCode
         */
        ReturnCode VerboseCodes(IOBuffer& input, IOBuffer& output);

        const Command<ReturnCode (Console::*)(IOBuffer& input, IOBuffer& output)> _options[2] =
        {
            {"help", &Console::Help},
            {"ccodes", &Console::VerboseCodes},
        };
    };

    using StandaloneCommand = Command<ReturnCode (*)(IOBuffer& input, IOBuffer& output)>;
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
