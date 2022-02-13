
#include <application/application.hpp>


bool Application::Init()
{
    _selector.Init();
    for(auto& but : fav)
    {
        but.Init();
    }
    return true;
}

// void Selector::Click()
// {
//     Message msg;
//     msg.type = MessageType::EncoderDepressed;
//     app.events.Push(msg);
// }

void Selector::Rotation(const RotaryEncoder::Event& evt)
{
    // Message msg;
    // msg.type = MessageType::EncoderRotation;
    // msg.elapsed = evt.elapsed;
    // msg.encoder_clockwise = evt.clock_wise;
    // app.events.Push(msg);
}

void Favorite::ShortClick()
{
    // Message msg;
    // msg.type = MessageType::FavoriteDepressed;
    // msg.favorite_id = id;
    // app.events.Push(msg);
}

Application app;
