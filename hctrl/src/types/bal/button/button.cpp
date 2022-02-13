#include "button.hpp"


namespace Button
{
    bool Button::Init(bool active_high, bool pull_opposite)
    {
        using namespace types::GPIO;
        auto opt = Options(Direction::Input);
        auto pull = active_high ? PullResistor::Down : PullResistor::Up;
        opt.pull_res = pull_opposite ? pull : PullResistor::None;
        /* Since we are the only binders to the pin, any ID will do. */
        const auto id = 0x00;
        auto rise_fall = active_high ? InterruptMode::Rising : InterruptMode::Falling;
        auto isr = IsrOptions(rise_fall, id);
        isr.pcallback_object = static_cast<types::GPIO::ICallback*>(this);
        _pin.Configure(opt, isr);
        return true;
    }

    bool Button::CallbackRegister(ICallback& user_callback)
    {
        _user_callback = &user_callback;
        return true;
    }

    void Button::CallbackClear()
    {
        _user_callback = nullptr;
    }

    bool Button::EventConsumerRegister(interfaces::Event::IConsumer& consumer, interfaces::Event::ProducerID id)
    {
        _event_consumer = &consumer;
        _producer_id = id;
    }

    void Button::EventConsumerClear()
    {
        _event_consumer = nullptr;
        _producer_id = 0x00;
    }

    EventType Button::GetEvent(interfaces::Event::Message& message)
    {
        return static_cast<EventType>(message.data);
    }

    /* Callback from button; will run on different thread! */
    void Button::Interrupt(types::CallbackId_t id)
    {
        // Some logic is required here, for interpreting the clicks
        if (_event_consumer)
        {
            interfaces::Event::Message msg = {
                .producer_id = _producer_id,
                .data =  static_cast<uint32_t>(EventType::ShortClick)
            };
            _event_consumer->StoreIncomingMessage(msg);
        }
    }

} /* namespace Button */
