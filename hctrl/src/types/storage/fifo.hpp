/*
 * fifo.hpp
 *
 *  Created on: Sep 26, 2021
 *      Author: ricardo
 */


#pragma once

#include <cstdint>
#include <cstring>
#include <cstdlib>


namespace FIFO
{
    template<typename T, size_t SIZE>
    class FIFO
    {
    public:
        FIFO()
        : _index(0)
        {
        }
        bool Push(T& value)
        {
            if (_index < SIZE)
            {
                _buffer[_index] = value;
                ++_index;
                return true;
            }
            return false;
        }
        T Pop()
        {
            if (_index > 0)
            {
                --_index;
                return _buffer[_index];
            }
            return {};
        }
    private:
        T _buffer[SIZE];
        size_t _index;
    };

}
