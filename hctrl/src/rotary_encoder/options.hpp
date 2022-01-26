#pragma once

#include <cstdint>

namespace RotaryEncoder
{
    namespace Options
    {
        /* Back-off time in ms to filter button bounce */
        const uint32_t ClickBackoff = 50;
        /* Back-off time in ms to filter button bounce */
        const uint32_t RotateBackoff = 15;
    }
}
