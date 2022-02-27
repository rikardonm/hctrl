#pragma once

#include <cstdint>

namespace Log
{

    using LevelType = uint8_t;

    enum class Level : LevelType
    {
        Trace = 0x00,
        Debug = 0x10,
        Info = 0x20,
        Warning = 0x30,
        Error = 0x40,
        Critical = 0x50
    };

} /* namespace Log */
