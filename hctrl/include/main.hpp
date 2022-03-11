
#pragma once

#include <fwork/base.hpp>


enum class OutputTypes
{
    NokiaText,
};


using State=fwork::State<Message, OutputTypes>;
