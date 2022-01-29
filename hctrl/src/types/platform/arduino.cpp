
#include <types/platform/arduino.hpp>

namespace types
{
namespace platforms
{
namespace arduino
{
    /**************************************************************************
     *                  GPIO
     *************************************************************************/
    bool Pin::Init(const types::GPIO::Options options)
    {
        return Configure(options);
    }

    bool Pin::Configure(const types::GPIO::Options options)
    {
        uint32_t a_mode;
        switch (options.direction)
        {
            case types::GPIO::Direction::Input:
                switch(options.pull_res)
                {
                    case types::GPIO::PullResistor::Up:
                        a_mode = INPUT_PULLUP;
                        break;
                    case types::GPIO::PullResistor::Down:
                        a_mode = INPUT_PULLDOWN;
                        break;
                    case types::GPIO::PullResistor::None:
                        a_mode = INPUT;
                        break;
                    default:
                        a_mode = INPUT;
                        break;
                }
                break;
            case types::GPIO::Direction::OutputPushPull:
                a_mode = OUTPUT;
                break;
            case types::GPIO::Direction::OutputOpenDrain:
                a_mode = OUTPUT_OPEN_DRAIN;
                break;
            default:
                a_mode = INPUT;
                break;
        }
        pinMode(_pin_num, a_mode);
        return true;
    }

    bool Pin::Configure(const types::GPIO::IsrOptions isr_options)
    {
        _user_callback = isr_options;
        auto arduino_mode = RISING;
        switch(_user_callback.interrupt_mode)
        {
            case types::GPIO::InterruptMode::BothEdges:
                arduino_mode = CHANGE;
                break;
            case types::GPIO::InterruptMode::Rising:
                arduino_mode = RISING;
                break;
            case types::GPIO::InterruptMode::Falling:
                arduino_mode = FALLING;
                break;
            case types::GPIO::InterruptMode::Low:
                arduino_mode = LOW;
                break;
            case types::GPIO::InterruptMode::High:
                arduino_mode = HIGH;
                break;
            default:
                arduino_mode = RISING;
                break;
        }
        attachInterrupt(digitalPinToInterrupt(_pin_num), _callback, arduino_mode);
        return true;
    }

    bool Pin::Configure(const types::GPIO::Options options, const types::GPIO::IsrOptions isr_options)
    {
        Configure(options);
        Configure(isr_options);
        return true;
    }

    void Pin::Set(bool value)
    {
        digitalWrite(_pin_num, value);
    }

    bool Pin::Read()
    {
        return digitalRead(_pin_num);
    }

    /**************************************************************************
     *                  SPI
     *************************************************************************/
    bool SPI::Init(const types::SPI::Options options)
    {
        return SetOptions(options);
    }

    void SPI::Transfer(uint8_t data)
    {
        _instance.transfer(data);
    }

    void SPI::Transfer(iArray<uint8_t> data)
    {
        if (false)
        {
            // Re-configure options
            _instance.setBitOrder(BitOrder::MSBFIRST);
        }
        _instance.transfer(data.data, data.size);
    }

    bool SPI::ApplyOptions()
    {
        _instance.begin();
        _instance.setBitOrder(_options.bit_order == types::SPI::BitOrder::MSBFirst ?  MSBFIRST : LSBFIRST);
        _instance.setDataMode(SPI_MODE0);
        // TODO: how do we calculate the divider?
        _instance.setClockDivider(SPI_CLOCK_DIV64);
        return true;
    }

} /* namespace arduino */
} /* namespace platforms */
} /* namespace types */
