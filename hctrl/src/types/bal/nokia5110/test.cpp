
#include "test.hpp"

namespace Nokia5110
{
    namespace Test
    {
        void FillPattern(Nokia5110& device, uint8_t pattern)
        {
            device.Position(0, 0);
            for(auto i = 0; i < device.GetBufferLength(); ++i)
            {
                device.PushToBuffer(pattern);
            }
        }
    } /* namespace Test */

} /* namespace Nokia5110 */
