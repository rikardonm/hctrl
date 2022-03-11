
#include <main.hpp>
#include <fwork/base.hpp>

// class MenuItem
// {
// public:
//     MenuItem(const char* name)
//     : _name(name)
//     {
//     }
//     const char* Name()
//     {
//         return _name;
//     }
// private:
//     const char* _name;
// };


// struct IntegerItem : public MenuItem
// {
// public:

//     IntegerItem(fwork::IntegerVariable<uint32_t>& variable_ref, const char* name)
//     : MenuItem(name), variable(variable_ref)
//     {
//     }
//     fwork::IntegerVariable<uint32_t>& variable;
// };




// IntegerItem home_items[] = {
//         {app.state.desired_height, "Fuck2"},
//         {app.state.favorites[0], "F0"},
//         {app.state.favorites[1], "F1"},
//         {app.state.favorites[2], "F2"}
// };


// enum class OutputTypes
// {
//     Nokia5110,
// };

// using State=fwork::State<Message, OutputTypes>;


// class Home : public State
// {
// public:
//     virtual void ProcessMessage(uint32_t timestamp, Message message) override;
//     virtual void UpdateTick(uint32_t timestamp) override;
//     virtual void UpdateOutput(uint32_t timestamp, OutputTypes output_type, uint32_t output_id) override;
// private:
//     uint32_t _menu_item_index = 0;
//     bool _is_editing = false;
//     TimedBackoff _recent_interaction = TimedBackoff(5000);
// };


// Home current_state = Home();

// void Home::ProcessMessage(uint32_t timestamp, Message msg)
// {
//     switch(msg.type)
//     {
//         case MessageType::EncoderDepressed:
//             _recent_interaction.Remark(timestamp);
//             _is_editing = !_is_editing;
//             break;
//         case MessageType::EncoderRotation:
//             _recent_interaction.Remark(timestamp);
//             if (_is_editing)
//             {
//                 if (msg.encoder_clockwise)
//                 {
//                     home_items[_menu_item_index].variable += 1;
//                 }
//                 else if (_menu_item_index > 0)
//                 {
//                     home_items[_menu_item_index].variable -= 1;
//                 }
//             }
//             else
//             {
//                 if (msg.encoder_clockwise)
//                 {
//                     if (_menu_item_index < Length(home_items) - 1)
//                     {
//                         _menu_item_index += 1;
//                     }
//                 }
//                 else if (_menu_item_index > 0)
//                 {
//                     _menu_item_index -= 1;
//                 }
//             }
//             break;
//         case MessageType::FavoriteDepressed:
//             switch(msg.favorite_id)
//             {
//                 case 0:
//                 case 1:
//                 case 2:
//                     app.state.desired_height = app.state.favorites[msg.favorite_id];
//                     BSP::console.StdOut() << "Favorite " << '0' + msg.favorite_id << " pressed.";
//                     BSP::console.StdOut().InsertNewLine();
//                     break;
//                 default:
//                     break;
//             }
//             break;
//     }
// }

// void Home::UpdateTick(uint32_t timestamp)
// {
// }

// void Home::UpdateOutput(uint32_t timestamp, OutputTypes output_type, uint32_t output_id)
// {
//     auto& lcd = BSP::lcd;
//     for (uint32_t i = 0; i < Length(home_items); ++i)
//     {
//         auto& item = home_items[i];
//         auto selected = _menu_item_index == i;
//         if (_is_editing or not _recent_interaction.isExpired(timestamp))
//         {
//             lcd.Invert(selected);
//         }
//         lcd.Position(0, i);
//         if (selected and _is_editing)
//         {
//             lcd << ">";
//         }
//         else
//         {
//             lcd << " ";
//         }
//         lcd << item.Name() << ": " << item.variable.Value();
//         lcd.PadLine();
//     }
//     lcd.Flush(true);
// }


