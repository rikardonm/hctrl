#include "ascii.hpp"


namespace ASCII
{

    uint8_t GetCodeCaretNotationChar(uint8_t code)
    {
        if (IsControlCode(code))
        {
            /* Caret code character is based at 0x40 '@' symbol, and wraps around at 8 bits */
            return '@' + code;
        }
        return code;
    }

    bool IsControlCode(uint8_t code)
    {
        return code <= ControlCodes::UnitSeparator or code == ControlCodes::Delete;
    }

} /* namespace ASCII */
