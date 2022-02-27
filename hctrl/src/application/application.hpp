

#pragma once


#include <cstdint>

#include <types/interfaces/event.hpp>
#include <types/storage/circular_buffer.hpp>
#include <types/storage/fifo.hpp>

#include <fwork/base.hpp>

#include <bal/user.hpp>
#include <abm/abm.hpp>


using EventBuffer_t = CircularBuffer<interfaces::Event::Message, 16, false>;


/* Favorite buttons */
struct Favorite : private Button::Button::ICallback
{
    Favorite(types::CallbackId_t id, Button::Button& button)
        : id(id), button(button)
    {}
    bool Init()
    {
        button.CallbackRegister(*this);
        return true;
    }

    virtual void ShortClick() override;
    virtual void LongClick() override
    {
        ShortClick();
    }
    virtual void DoubleClick() override
    {
        ShortClick();
    }

    types::CallbackId_t id;
    Button::Button& button;
};

/* Rotary Encoder Menu Selector */
struct Selector : public RotaryEncoder::RotaryEncoder::ICallback
{
    Selector(RotaryEncoder::RotaryEncoder& encoder)
        : encoder(encoder)
    {
    }
    bool Init()
    {
        encoder.Configure(static_cast<RotaryEncoder::RotaryEncoder::ICallback&>(*this));
        return true;
    }
    virtual void Rotation(const RotaryEncoder::Event& msg) override;
    RotaryEncoder::RotaryEncoder& encoder;
};


class Application
{
public:
    Application()
        : events(),
        state(),
        _select_click(ABM::selector_button),
        _selector(BSP::selector),
        fav({{0x01, ABM::favorite_1}, {0x02, ABM::favorite_2}, {0x03, ABM::favorite_3}})
    {}
    bool Init();

    EventBuffer_t events;
private:
    struct ApplicationState
    {
        uint32_t distance = 0;
        fwork::IntegerVariable<uint32_t> desired_height = {120, 250};
        fwork::IntegerVariable<uint32_t> favorites[3] = {
            {120, 250, 120},
            {120, 250, 180},
            {120, 250, 230}
        };
    } state;
    /* HMI */
    Button::Button& _select_click;
    Selector _selector;
    Favorite fav[3];
};

extern Application app;
