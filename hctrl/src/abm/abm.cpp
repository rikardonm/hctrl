
#include "abm.hpp"

#include <bal/user.hpp>
#include <types/abm/dotmatrix/text_typefaces.hpp>

const uint32_t ConsoleBaudRate = 115200;

dotmatrix::Text ABM::text_out(BSP::lcd);

// TODO: reference something from platform!
Console::Console ABM::console(Serial1);

void ABM::Init()
{
    BSP::Init();
    text_out.Init();
    // Console debug + human interaction
    console.Init(ConsoleBaudRate);
}
