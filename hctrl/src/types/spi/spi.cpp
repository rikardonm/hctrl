
#include "spi.hpp"


namespace types
{
namespace SPI
{

    bool SPI::SetOptions(const Options options)
    {
        return ApplyOptions();
    }

    Options SPI::GetOptions()
    {
        return options;
    }

} /* namespace SPI */
} /* namespace types */
