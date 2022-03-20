/*
 * base.hpp
 *
 *  Created on: Oct 3, 2021
 *      Author: ricardo
 */


#pragma once

#include <cstdint>


namespace fwork
{

    template<typename MESSAGE_TYPES, typename OUTPUT_TYPES>
    class State
    {
    public:
        State() = default;
        virtual ~State() = default;
        virtual void ProcessMessage(uint32_t timestamp, MESSAGE_TYPES message) = 0;
        virtual void UpdateTick(uint32_t timestamp);
        virtual void UpdateOutput(uint32_t timestamp, OUTPUT_TYPES output_type, uint32_t output_id) = 0;
    };

    template<typename T>
    class IntegerVariable
    {
    public:
        using _Base = IntegerVariable<T>;
        IntegerVariable(const T min, const T max)
        : _value(min), _min(min), _max(max)
        {
        }
        IntegerVariable(const T min, const T max, T value)
        : _value(value), _min(min), _max(max)
        {
        }
        _Base& operator+=(T value)
        {
            if ((_max - _value) > value)
            {
                _value += value;
            }
            return *this;
        }
        _Base& operator-=(T value)
        {
            if (_value >= (_min + value))
            {
                _value -= value;
            }
            return *this;
        }
        T Value()
        {
            return _value;
        }
        _Base& operator=(_Base& value)
        {
            if (this != &value)
            {
                *this = value._value;
            }
            return *this;
        }
        _Base& operator=(T value)
        {
            if (_min <= value and value < _max)
            {
                _value = value;
            }
            return *this;
        }
    private:
        T _value;
        const T _min;
        const T _max;
    };

}
