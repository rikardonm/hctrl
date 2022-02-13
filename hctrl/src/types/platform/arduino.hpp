#pragma once

#include <types/hal/gpio/gpio.hpp>
#include <types/hal/spi/spi.hpp>

#include <Arduino.h>
#include <SPI.h>

namespace types
{
namespace platforms
{
namespace arduino
{

    /**************************************************************************
     *                  GPIO
     *************************************************************************/
    class Pin : public types::GPIO::Pin
    {
    public:
        Pin(const uint32_t num, void (*callback)(void))
        : types::GPIO::Pin(),
            _pin_num(num),
            _callback(callback)
        {}

        virtual bool Init(const types::GPIO::Options& options) override;
        virtual bool Configure(const types::GPIO::Options& options) override;
        virtual bool Configure(const types::GPIO::IsrOptions& isr_options) override;
        virtual bool Configure(const types::GPIO::Options& options, const types::GPIO::IsrOptions& isr_options) override;

        virtual void Set(const bool value) override;
        virtual bool Read() override;

        void Callback()
        {
            if (_user_callback.pcallback_object)
            {
                _user_callback.pcallback_object->Interrupt(_user_callback.id);
            }
            else if (_user_callback.pevent_consumer)
            {
                interfaces::Event::Message msg = {.producer_id = _user_callback.id};
                _user_callback.pevent_consumer->StoreIncomingMessage(msg);
            }
        }

        virtual ~Pin() = default;
    private:
        bool ConfigureInterrupt(const types::GPIO::IsrOptions isr_options);

        const uint32_t _pin_num;
        const types::FreeFunctionIsr _callback;
    };

    /**************************************************************************
     *                  SPI
     *************************************************************************/
    class SPI : public types::SPI::SPI
    {
    public:
        SPI(SPIClass& instance) : _instance(instance) {}

        virtual bool Init(const types::SPI::Options options) override;
        virtual void Transfer(uint8_t data) override;
        virtual void Transfer(iArray<uint8_t> data) override;

        virtual ~SPI() = default;
    protected:
        virtual bool ApplyOptions() override;

    private:
        /* Store Arduino's instance */
        SPIClass& _instance;
    };

} /* namespace arduino */
} /* namespace platforms */
} /* namespace types */
