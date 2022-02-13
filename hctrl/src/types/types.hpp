
#pragma once

#include <cstdint>
#include <cstddef>


namespace types
{

    using CallbackId_t = uint32_t;
    using IsrCallback_t = void (*)(CallbackId_t id);
    using FreeFunctionIsr = void (*)(void);

} /* namespace types */
