#pragma once

#include <cstdint>
#include <memory>

#include <types/types.hpp>

namespace types
{
namespace GPIO
{
    enum class Direction
    {
        Input,
        OutputPushPull,
        OutputOpenDrain,
    };

    enum class PullResistor
    {
        Up = -0x10,
        None = 0x00,
        Down = 0x10,
    };

    /**
     * @brief
     *
     * Declare the values with gaps, so intermediate values can be added if need be.
     *
     */
    enum class DriveStrength
    {
        Weak = -0x10,
        Normal = 0x00,
        Strong = 0x10,
    };

    enum class SlewRate
    {
        Slow = -0x10,
        Normal = 0x00,
        Fast = 0x10,
    };

    enum class InterruptMode
    {
        None,
        Rising,
        Falling,
        BothEdges,
        High,
        Low,
    };

    struct Options
    {
        Direction direction;
        PullResistor pull_res;
        DriveStrength drive_strength;
        SlewRate slew_rate;
        Options(
            Direction dir,
            PullResistor pull = PullResistor::None,
            DriveStrength drive = DriveStrength::Normal,
            SlewRate slew = SlewRate::Normal
        ) : direction(dir), pull_res(pull), drive_strength(drive), slew_rate(slew)
        {}
    };

    class Callback
    {
    public:
        virtual void Interrupt(CallbackId_t) = 0;
        using ptr = Callback*;
    };

    struct IsrOptions
    {
        InterruptMode interrupt_mode;
        CallbackId_t id;
        Callback::ptr callback_object;
        IsrOptions(InterruptMode mode, CallbackId_t id, Callback::ptr object)
            : interrupt_mode(mode), id(id), callback_object(object)
        {}
        IsrOptions(InterruptMode mode, CallbackId_t id, Callback& object)
        {
            IsrOptions(mode, id, &object);
        }
        operator bool()
        {
            return callback_object;
        }
    };

    class Pin : protected Owned
    {
    public:
        Pin() : Owned(), _user_callback(InterruptMode::Rising, 0x00, nullptr) {}

        virtual bool Init(const Options options) = 0;
        virtual bool Configure(const Options options) = 0;
        virtual bool Configure(const IsrOptions isr_options) = 0;
        virtual bool Configure(const Options options, const IsrOptions isr_options) = 0;

        virtual void Set(const bool value) = 0;
        virtual bool Read() = 0;

        virtual ~Pin() = default;
    protected:
        IsrOptions _user_callback;
    };

} /* namespace GPIO */
} /* namespace types */
