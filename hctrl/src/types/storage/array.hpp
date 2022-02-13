
#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>

namespace types
{
    template<typename T, std::size_t SIZE>
    struct Array
    {
        T data[SIZE];
        std::size_t size;
        void Zerofy()
        {
            memset(data, 0x00, Bytes());
        }
        constexpr size_t Bytes()
        {
            return sizeof(data);
        }
    };

    template<typename T>
    struct iArray
    {
        T* data;
        std::size_t size;

        template<std::size_t SIZE>
        constexpr iArray(T data[SIZE]) : data(data), size(SIZE) {}

        template<std::size_t SIZE>
        constexpr iArray(Array<uint8_t, SIZE>& array) : data(array.data), size(array.size) {}
    };
} /* namespace types */
