
#include "log.hpp"
#include "options.hpp"

#include <abm/abm.hpp>

namespace Log
{

    void ConsoleLogger::PushToBackend(const char* str)
    {
        auto& out = ABM::console.StdOut();
        out.InsertString(str);
    }


    ConsoleLogger console = ConsoleLogger();

} /* namespace Log */
