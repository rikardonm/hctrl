#pragma once

#include <cstdint>

namespace RotaryEncoder
{
    namespace Options
    {
        /* Backoff time in ms to filter button bounce */
        const uint32_t ClickBackoff = 50;
        /* Backoff time in ms to filter button bounce */
        const uint32_t RotateBackoff = 5;
    }
}