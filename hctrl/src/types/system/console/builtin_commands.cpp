#include "console.hpp"


namespace Console
{

    const StandaloneCommand empty_command = {nullptr, nullptr};

    types::ReturnCode Run(IOBuffer& input, IOBuffer& output)
    {
        /* Input buffer data is consumed in the operation */
        output << "Testing! Echo: ";
        //  << input;
        // todo: fixme!
        return types::ReturnCode::Success;
    }

    types::ReturnCode Help(IOBuffer& input, IOBuffer& output)
    {
        for (auto i = 0; ; ++i)
        {
            auto& cmd = builtin_commands[i];
            if (not cmd.name)
            {
                break;
            }
            auto& next = builtin_commands[i+1];
            output.InsertString(cmd.name, next.name);
        }
        return types::ReturnCode::Success;
    }

    types::ReturnCode Reset(IOBuffer& input, IOBuffer& output)
    {
        HAL_NVIC_SystemReset();
        return types::ReturnCode::Success;
    }

    bool RegisterNewCommand(StandaloneCommand newcommand)
    {
        for (auto i = 0; i < Options::BuiltinCommandsSize; ++i)
        {
            auto& cmd = builtin_commands[i];
            if (not cmd.name)
            {
                cmd = newcommand;
                return true;
            }
        }
        return false;
    }

    StandaloneCommand builtin_commands[Options::BuiltinCommandsSize + 1] =
    {
        {"help", Help},
        {"echo", Run},
        {"reset", Reset},
        empty_command,
    };

} /* namespace Console */
