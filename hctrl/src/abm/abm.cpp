
#include "abm.hpp"

#include <bal/user.hpp>
#include <types/abm/dotmatrix/text_typefaces.hpp>

#include <assets/azevem_icon.hpp>


const uint32_t ConsoleBaudRate = 115200;

dotmatrix::Text ABM::text_out(BSP::lcd);

// TODO: reference something from platform!
Console::Console ABM::console(Serial1);


void ABM::Init()
{
    BSP::Init();

    BSP::lcd.CopyBuffer(types::iArray<const uint8_t>(assets::azevem_icon, sizeof(assets::azevem_icon)));
    /* Ideally no copy would be preferred, but the SPI function writes back
    in the same buffer. */
    BSP::lcd.Flush();

    text_out.Init();
    // Console debug + human interaction
    console.Init(ConsoleBaudRate);
}
