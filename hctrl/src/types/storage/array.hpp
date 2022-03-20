
#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>

namespace types
{

    template<typename T>
    struct iArray
    {
        T* const data;
        const std::size_t size;

        // PROBLEM
        // template<std::size_t SIZE>
        // constexpr explicit iArray(T data[SIZE]) : data(data), size(SIZE) {}

        constexpr explicit iArray(T* data, std::size_t size) : data(data), size(size) {}
    };

    template<typename T, std::size_t SIZE>
    struct Array
    {
        using Base=Array<T, SIZE>;

        T data[SIZE];
        const std::size_t size = SIZE;

        void Zerofy()
        {
            memset(data, 0x00, LengthBytes());
        }
        constexpr size_t LengthBytes()
        {
            return sizeof(data);
        }
        Base& operator=(const Array<const T, SIZE>& other)
        {
            memcpy(data, other.data, SIZE);
            return *this;
        }
        Base& operator=(const iArray<T>& other)
        {
            memcpy(data, other.data, other.size);
            return *this;
        }
        operator iArray<T>()
        {
            return iArray<T>(data, SIZE);
        }
    };

} /* namespace types */
