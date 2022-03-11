#pragma once

#include <cstdint>
#include <cstddef>

namespace Console
{
    namespace Options
    {
        /* Length of input & output circular buffers. */
        const size_t BufferLength         = 512;
        /* Option to clean data after it has been retrieved or discarted from the buffer. */
        const bool CleanBuffer            = false;
        /* */
        const char ConsoleHeader[]        = "-> ";
        /* Print logo at startup. */
        const bool PrintLogo              = true;
        /* Print banner at startup. */
        const bool PrintBanner            = false;
        /* Fixed size of availble console commands, including built-in commands. */
        const size_t BuiltinCommandsSize  = 15;
    }
} /* namespace Console */
