
#include "spi.hpp"


namespace types
{
namespace SPI
{

    bool SPI::SetOptions(const Options options)
    {
        _options = options;
        return ApplyOptions();
    }

    Options SPI::GetOptions()
    {
        return _options;
    }

} /* namespace SPI */
} /* namespace types */
