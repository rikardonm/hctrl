
#pragma once

#include <types/types.hpp>
#include <types/interfaces/event.hpp>
#include <types/hal/gpio/gpio.hpp>

namespace Button
{
    enum class EventType : uint8_t
    {
        ShortClick,
        LongClick,
        DoubleClick,
    };

    class Button : private types::GPIO::ICallback
    {
    public:
        struct ICallback
        {
            virtual void ShortClick() = 0;
            virtual void LongClick() = 0;
            virtual void DoubleClick() = 0;
            virtual ~ICallback() = default;
            using ptr = ICallback*;
        };

        Button(types::GPIO::Pin& pin) : _pin(pin)
        {}
        /**
         * @brief Configure the button.
         *
         * @param active_high Logic level at pin input when the button is depressed.
         * @param pull_opposite Enable pull up/down internal resistors.
         * @return true If configuration was successful.
         * @return false If error ocurred.
         */
        bool Init(bool active_high, bool pull_opposite);
        bool CallbackRegister(ICallback& user_callback);
        void CallbackClear();
        bool EventConsumerRegister(interfaces::Event::IConsumer& consumer, interfaces::Event::ProducerID id);
        void EventConsumerClear();
        EventType GetEvent(interfaces::Event::Message& message);
    private:
        ICallback::ptr _user_callback;
        interfaces::Event::IConsumer::ptr _event_consumer;
        interfaces::Event::ProducerID _producer_id;
        types::GPIO::Pin& _pin;
        virtual void Interrupt(types::CallbackId_t id) override;
    };

} /* namespace Button */
