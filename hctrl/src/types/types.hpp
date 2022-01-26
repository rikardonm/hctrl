
#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>


namespace types
{

    using CallbackId_t = uint32_t;
    using IsrCallback_t = void (*)(CallbackId_t id);
    using FreeFunctionIsr = void (*)(void);

    class Owned
    {
    public:
        Owned() : _last_id(nullptr) {}
    private:
        void* _last_id;
    };

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
