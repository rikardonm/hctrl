#pragma once

#include <cstdint>

namespace ASCII
{
    /**
     * @brief List of ASCII control codes used in serial communications.
     *
     * @details Data sourced from https://en.wikipedia.org/wiki/ASCII
     */
    enum ControlCodes : uint8_t
    {
        Null                    = 0x00,     // C esacpe: '\0'
        StartOfHeading          = 0x01,
        StartOfText             = 0x02,
        EndOfText               = 0x03,
        EndOfTransmission       = 0x04,
        Enquiry                 = 0x05,
        Acknowledgement         = 0x06,
        Bell                    = 0x07,     // C esacpe: '\a'
        Backspace               = 0x08,     // C esacpe: '\b'
        HorizontalTab           = 0x09,     // C esacpe: '\t'
        LineFeed                = 0x0A,     // C esacpe: '\n'
        VerticalTab             = 0x0B,     // C esacpe: '\v'
        FormFeed                = 0x0C,     // C esacpe: '\f'
        CarriageReturn          = 0x0D,     // C esacpe: '\r'
        ShiftOut                = 0x0E,
        ShiftIn                 = 0x0F,
        DataLinkEscape          = 0x10,
        DeviceControl1Xon       = 0x11,
        DeviceControl2          = 0x12,
        DeviceControl3Xoff      = 0x13,
        DeviceControl4          = 0x14,
        NegativeAcknowledgement = 0x15,
        SynchronousIdle         = 0x16,
        EndOfTransmissionBlock  = 0x17,
        Cancel                  = 0x18,
        EndOfMedium             = 0x19,
        Substitute              = 0x1A,
        Escape                  = 0x1B,     // C esacpe: '\e'
        FileSeparator           = 0x1C,
        GroupSeparator          = 0x1D,
        RecordSeparator         = 0x1E,
        UnitSeparator           = 0x1F,
        Delete                  = 0x7F,
    };

    /**
     * @brief Get character in caret notation.
     *
     * Returns character to print after the '^' symbol, e.g. 'C' in '^C'.
     *
     * @param code ASCII control code
     * @return char character after '^' symbol
     */
    uint8_t GetCodeCaretNotationChar(uint8_t code);

    /**
     * @brief
     *
     * @param code
     * @return true
     * @return false
     */
    bool IsControlCode(uint8_t code);

} /* namespace ASCII */
