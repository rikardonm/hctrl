
#include "console.hpp"

namespace Console
{
    ReturnCode Run(IOBuffer& input, IOBuffer& output)
    {
        /* Input buffer data is consumed in the operation */
        output << "Testing!:" << input;
        return ReturnCode::Success;
    }

    ReturnCode Help(IOBuffer& input, IOBuffer& output)
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
        return ReturnCode::Success;
    }

    StandaloneCommand builtin_commands[Options::BuiltinCommandsSize + 1] =
    {
        {"help", Help},
        {"echo", Run},
        {nullptr, nullptr},
    };

} /* namespace Console */
