
#pragma once

#include <types/types.hpp>

namespace types
{
namespace SPI
{

    enum class BitOrder
    {
        MSBFirst,
        LSBFirst
    };

    enum class DataLength
    {
        Bits8,
        Bits7
    };

    enum class ClockPolarityLead : uint8_t
    {
        Rising = 0x00,
        Falling = 0x01
    };

    enum class ClockPhaseSample : uint8_t
    {
        OutTrailing = 0x00,
        OutLeading = 0x10
    };

    enum class TransferMode : uint8_t
    {
        Mode_0 = 0x00,
        Mode_1 = 0x01,
        Mode_2 = 0x10,
        Mode_3 = 0x11
    };

    constexpr TransferMode ModeFromPolarityPhase(ClockPolarityLead pol, ClockPhaseSample pha)
    {
        return static_cast<TransferMode>(static_cast<uint8_t>(pol) | static_cast<uint8_t>(pha));
    }

    /**
     * @brief SPI Options
     *
     */
    struct Options
    {
        uint32_t clock_speed;
        TransferMode mode;
        BitOrder bit_order;
        DataLength data_length;
        bool software_ssn;
        Options(uint32_t clock = 1'000'000, TransferMode mode = TransferMode::Mode_0, BitOrder order = BitOrder::MSBFirst, DataLength length = DataLength::Bits7)
            : clock_speed(clock), mode(mode), bit_order(order), data_length(length), software_ssn(false)
        {}
    };

    class SPI : protected Owned
    {
    public:
        SPI() : Owned() {}

        bool SetOptions(const Options options);
        Options GetOptions();

        virtual bool Init(const Options options) = 0;
        virtual void Transfer(uint8_t data) = 0;
        virtual void Transfer(iArray<uint8_t> data) = 0;

        virtual ~SPI() = default;
    protected:
        virtual bool ApplyOptions() = 0;

        /* Options is private as if the user changes it, the block is made aware of it */
        Options options;
    };

} /* namespace SPI */
} /* namespace types */
