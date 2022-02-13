#pragma once

#include <cstdint>
#include <cstddef>


namespace types
{

    template<typename T, size_t SIZE>
    constexpr size_t Length(const T (&array)[SIZE])
    {
        return SIZE;
    }

} /* namespace types */
