#pragma once

#include <cstdint>
#include <memory>

#include <types/types.hpp>
#include <types/objects.hpp>
#include <types/interfaces/event.hpp>

namespace types
{
namespace GPIO
{
    /**
     * @brief IO control direction.
     *
     */
    enum class Direction
    {
        Input,
        OutputPushPull,
        OutputOpenDrain,
    };

    /**
     * @brief Internal pull resistor.
     *
     */
    enum class PullResistor
    {
        Up = -0x10,
        None = 0x00,
        Down = 0x10,
    };

    /**
     * @brief Hardware drive strength (current sink/source capabilities).
     *
     * Values are declared with gaps, so intermediate values can be added if need be.
     */
    enum class DriveStrength
    {
        Weak = -0x10,
        Normal = 0x00,
        Strong = 0x10,
    };

    /**
     * @brief IO slew rate configuration.
     *
     */
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

    class ICallback
    {
    public:
        virtual void Interrupt(CallbackId_t) = 0;
        virtual ~ICallback() = default;
        using ptr = ICallback*;
    };

    struct IsrOptions
    {
        InterruptMode interrupt_mode;
        CallbackId_t id;
        ICallback::ptr pcallback_object;
        interfaces::Event::IConsumer::ptr pevent_consumer;
        IsrOptions(InterruptMode mode, CallbackId_t id)
            : interrupt_mode(mode), id(id), pcallback_object(nullptr), pevent_consumer(nullptr)
        {
        }
    };

    class Pin : protected Owned
    {
    public:
        Pin() : Owned(), _user_callback(InterruptMode::Rising, 0x00) {}

        virtual bool Init(const Options& options) = 0;
        virtual bool Configure(const Options& options) = 0;
        virtual bool Configure(const IsrOptions& isr_options) = 0;
        virtual bool Configure(const Options& options, const IsrOptions& isr_options) = 0;

        virtual void Set(const bool value) = 0;
        virtual bool Read() = 0;

        virtual ~Pin() = default;
    protected:
        IsrOptions _user_callback;
    };

} /* namespace GPIO */
} /* namespace types */
