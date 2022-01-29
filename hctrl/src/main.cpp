// #include <variant_BLUEPILL_F103C6.h>
// #include <stm32f103x6.h>

// #include <Arduino.h>

#include <cstdint>

#include <general/circular_buffer.hpp>
#include <general/fifo.hpp>
#include <general/helpers.hpp>

#include <fwork/base.hpp>

#include <bsp/user.hpp>


#include "blinky.hpp"



auto lcd_backoff = TimedBackoff(50);
const auto splash_screen_delay = 1000;

auto blinky = Blinky(*BSP::status_led, 500);


enum class MessageType
{
    FavoriteDepressed,
    EncoderDepressed,
    EncoderRotation,
};


struct Message
{
    MessageType type;
    union
    {
        uint32_t elapsed;
        uint32_t encoder_clockwise;
        uint32_t favorite_id;
    };
};


struct ApplicationState
{
    uint32_t distance = 0;
    fwork::IntegerVariable<uint32_t> desired_height = {120, 250};
    fwork::IntegerVariable<uint32_t> favorites[3] = {
        {120, 250, 120},
        {120, 250, 180},
        {120, 250, 230}
    };
};



class Application
{
public:
    Application()
        : events(),
        state(),
        _selector(*this, BSP::selector),
        fav({{0x01, *BSP::favorite1}, {0x02, *BSP::favorite2}, {0x03, *BSP::favorite3}})
    {}
    bool Init();


    CircularBuffer<Message, 16, false> events;
    ApplicationState state;
    /* HMI */
    Selector _selector;
    Favorite fav[3];

private:
    /* Rotary Encoder Menu Selector */
    struct Selector : public RotaryEncoder::Callback
    {
        Selector(Application& app, RotaryEncoder::RotaryEncoder& encoder)
            : app(app), encoder(encoder)
        {
            encoder;
        }
        bool Init()
        {
            encoder.Configure(*this);
            return true;
        }
        virtual void Click(uint32_t elapsed_ms) override;
        virtual void Rotation(uint32_t elapsed_ms, bool clockwise) override;
        Application& app;
        RotaryEncoder::RotaryEncoder& encoder;
    };

    /* Favorite buttons */
    struct Favorite : public types::GPIO::Callback
    {
        Favorite(types::CallbackId_t id, types::GPIO::Pin& pin) : id(id), backoff(150), pin(pin)
        {}
        bool Init()
        {
            pin.Configure(types::GPIO::IsrOptions(types::GPIO::InterruptMode::Rising, id, *this));
            return true;
        }
        virtual void Interrupt(types::CallbackId_t id) override;
        types::CallbackId_t id;
        TimedBackoff backoff;
        types::GPIO::Pin& pin;
    };
};

Application app;

bool Application::Init()
{
    _selector.Init();
    for(auto& pin : fav)
    {
        pin.Init();
    }
    return true;
}

void Application::Selector::Click(uint32_t elapsed_ms)
{
    Message msg;
    msg.type = MessageType::EncoderDepressed;
    msg.elapsed = elapsed_ms;
    app.events.Push(msg);
}

void Application::Selector::Rotation(uint32_t elapsed_ms, bool clockwise)
{
    Message msg;
    msg.type = MessageType::EncoderRotation;
    msg.elapsed = elapsed_ms;
    msg.encoder_clockwise = clockwise;
    app.events.Push(msg);
}

void Application::Favorite::Interrupt(types::CallbackId_t id)
{
    if (not backoff.Mark(millis()))
    {
        return;
    }
    Message msg;
    msg.type = MessageType::FavoriteDepressed;
    msg.elapsed = backoff.Elapsed();
    msg.favorite_id = id;
    app.events.Push(msg);
}


Console::ReturnCode SetFavorite(Console::IOBuffer& input, Console::IOBuffer& output)
{
    input.Discard(sizeof("setf"), false);
    auto [success, value] = input.Pop();
    if (not success)
    {
        return Console::ReturnCode::Unknown;
    }
    switch(value)
    {
        case '0':
            app.state.favorites[0] = 144;
            break;
        case '1':
            app.state.favorites[1] = 185;
            break;
        case '2':
            app.state.favorites[2] = 210;
            break;
        default:
            return Console::ReturnCode::Unknown;
    }
    return Console::ReturnCode::Success;
}

Console::StandaloneCommand SetFavoriteCommand = {"setf", SetFavorite};





class MenuItem
{
public:
    MenuItem(const char* name)
    : _name(name)
    {
    }
    const char* Name()
    {
        return _name;
    }
private:
    const char* _name;
};


struct IntegerItem : public MenuItem
{
public:

    IntegerItem(fwork::IntegerVariable<uint32_t>& variable_ref, const char* name)
    : MenuItem(name), variable(variable_ref)
    {
    }
    fwork::IntegerVariable<uint32_t>& variable;
};




IntegerItem home_items[] = {
        {app.state.desired_height, "Fuck2"},
        {app.state.favorites[0], "F0"},
        {app.state.favorites[1], "F1"},
        {app.state.favorites[2], "F2"}
};


enum class OutputTypes
{
    Nokia5110,
};

using State=fwork::State<Message, OutputTypes>;


class Home : public State
{
public:
    virtual void ProcessMessage(uint32_t timestamp, Message message) override;
    virtual void UpdateTick(uint32_t timestamp) override;
    virtual void UpdateOutput(uint32_t timestamp, OutputTypes output_type, uint32_t output_id) override;
private:
    uint32_t _menu_item_index = 0;
    bool _is_editing = false;
    TimedBackoff _recent_interaction = TimedBackoff(5000);
};


Home current_state = Home();

void Home::ProcessMessage(uint32_t timestamp, Message msg)
{
    switch(msg.type)
    {
        case MessageType::EncoderDepressed:
            _recent_interaction.Remark(timestamp);
            _is_editing = !_is_editing;
            break;
        case MessageType::EncoderRotation:
            _recent_interaction.Remark(timestamp);
            if (_is_editing)
            {
                if (msg.encoder_clockwise)
                {
                    home_items[_menu_item_index].variable += 1;
                }
                else if (_menu_item_index > 0)
                {
                    home_items[_menu_item_index].variable -= 1;
                }
            }
            else
            {
                if (msg.encoder_clockwise)
                {
                    if (_menu_item_index < Length(home_items) - 1)
                    {
                        _menu_item_index += 1;
                    }
                }
                else if (_menu_item_index > 0)
                {
                    _menu_item_index -= 1;
                }
            }
            break;
        case MessageType::FavoriteDepressed:
            switch(msg.favorite_id)
            {
                case 0:
                case 1:
                case 2:
                    app.state.desired_height = app.state.favorites[msg.favorite_id];
                    BSP::console.StdOut() << "Favorite " << '0' + msg.favorite_id << " pressed.";
                    BSP::console.StdOut().InsertNewLine();
                    break;
                default:
                    break;
            }
            break;
    }
}

void Home::UpdateTick(uint32_t timestamp)
{
}

void Home::UpdateOutput(uint32_t timestamp, OutputTypes output_type, uint32_t output_id)
{
    auto& lcd = BSP::lcd;
    for (uint32_t i = 0; i < Length(home_items); ++i)
    {
        auto& item = home_items[i];
        auto selected = _menu_item_index == i;
        if (_is_editing or not _recent_interaction.isExpired(timestamp))
        {
            lcd.Invert(selected);
        }
        lcd.Position(0, i);
        if (selected and _is_editing)
        {
            lcd << ">";
        }
        else
        {
            lcd << " ";
        }
        lcd << item.Name() << ": " << item.variable.Value();
        lcd.PadLine();
    }
    lcd.Flush(true);
}

void setup()
{
    BSP::Init();

    // Console::RegisterNewCommand(SetFavoriteCommand);

    app.Init();

    /* let the splash screens sink in... */
    delay(splash_screen_delay);
    blinky.Init();
    BSP::lcd.Clear();
}

void loop()
{
    auto mils = millis();
    blinky.EventLoopStep(mils);
    BSP::console.EventLoopStep(mils);
    // auto [success, value] = app.events.Pop();
    // if (success)
    // {
    //     current_state.ProcessMessage(mils, value);
    // }
    if (lcd_backoff.Mark(mils))
    {
        current_state.UpdateOutput(mils, OutputTypes::Nokia5110, 0);
    }
    current_state.UpdateTick(mils);
}
