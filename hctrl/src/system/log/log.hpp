

#pragma once

#include <cstdint>

#include <types/interfaces/string.hpp>
#include <types/numbers.hpp>

#include "definitions.hpp"
#include "options.hpp"


namespace Log
{
    /*
     * Logging scheme is composed of 2 parts: Frontend and backend. The frontend is responsible for formatting the input
     * into an ASCII printable string and forwarding it to the registered backends. The backend is responsible for
     * handling and registering this data.
     */
    class IBackend
    {
    public:
        virtual void Push(const char* str) = 0;
        virtual ~IBackend() = default;
    };

    struct OutputFormat
    {
        types::NumberBase number_base = types::NumberBase::Decimal;
        bool number_base_prefix = true;
    };

    class IFrontend
    {
    public:
        void Log(const char* str)
        {
            if (static_cast<LevelType>(_level) < static_cast<LevelType>(Options::LowestDebugLevel))
            {
                return;
            }
            PushToBackend(str);
        }

        void SetOutputFormat(const OutputFormat fmt)
        {
            _output_format = fmt;
        }
        OutputFormat GetOutoutFormat()
        {
            return _output_format;
        }
        void SetLevel(Level lvl)
        {
            _level = lvl;
        }
        Level GetLevel()
        {
            return _level;
        }
    private:
        Level _level = Level::Error;
        virtual void PushToBackend(const char* str) = 0;
        OutputFormat _output_format;
    };

    class ConsoleLogger : public IFrontend
    {
    private:
        virtual void PushToBackend(const char* str) override;
    };

    extern ConsoleLogger console;

} /* namespace Log */
