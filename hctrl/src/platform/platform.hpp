
#pragma once

#include <cstdint>

#include <types/platform/arduino.hpp>

namespace platform
{
    /**************************************************************************
     *                  GPIO
     *************************************************************************/
    extern types::platforms::arduino::Pin pin_0;
    void Callback_pin_0(void);
    extern types::platforms::arduino::Pin pin_1;
    void Callback_pin_1(void);
    extern types::platforms::arduino::Pin pin_2;
    void Callback_pin_2(void);
    extern types::platforms::arduino::Pin pin_3;
    void Callback_pin_3(void);
    extern types::platforms::arduino::Pin pin_4;
    void Callback_pin_4(void);
    extern types::platforms::arduino::Pin pin_5;
    void Callback_pin_5(void);
    extern types::platforms::arduino::Pin pin_6;
    void Callback_pin_6(void);
    extern types::platforms::arduino::Pin pin_7;
    void Callback_pin_7(void);
    // USB DP
    extern types::platforms::arduino::Pin pin_8;
    void Callback_pin_8(void);
    // USB DN
    extern types::platforms::arduino::Pin pin_9;
    void Callback_pin_9(void);
    extern types::platforms::arduino::Pin pin_10;
    void Callback_pin_10(void);
    extern types::platforms::arduino::Pin pin_11;
    void Callback_pin_11(void);
    extern types::platforms::arduino::Pin pin_12;
    void Callback_pin_12(void);
    extern types::platforms::arduino::Pin pin_13;
    void Callback_pin_13(void);
    extern types::platforms::arduino::Pin pin_14;
    void Callback_pin_14(void);
    extern types::platforms::arduino::Pin pin_15;
    void Callback_pin_15(void);
    extern types::platforms::arduino::Pin pin_16;
    void Callback_pin_16(void);
    extern types::platforms::arduino::Pin pin_17;
    void Callback_pin_17(void);
    extern types::platforms::arduino::Pin pin_18;
    void Callback_pin_18(void);
    extern types::platforms::arduino::Pin pin_19;
    void Callback_pin_19(void);

    extern types::platforms::arduino::Pin pin_30;
    void Callback_pin_30(void);
    extern types::platforms::arduino::Pin pin_31;
    void Callback_pin_31(void);
    // BOOT 1
    extern types::platforms::arduino::Pin pin_32;
    void Callback_pin_32(void);
    // SWDIO
    extern types::platforms::arduino::Pin pin_33;
    void Callback_pin_33(void);
    // SWDCLK
    extern types::platforms::arduino::Pin pin_34;
    void Callback_pin_34(void);

    extern types::platforms::arduino::Pin pin_0xC0;
    void Callback_pin_0xC0(void);
    extern types::platforms::arduino::Pin pin_0xC1;
    void Callback_pin_0xC1(void);
    extern types::platforms::arduino::Pin pin_0xC2;
    void Callback_pin_0xC2(void);
    extern types::platforms::arduino::Pin pin_0xC3;
    void Callback_pin_0xC3(void);
    extern types::platforms::arduino::Pin pin_0xC4;
    void Callback_pin_0xC4(void);
    extern types::platforms::arduino::Pin pin_0xC5;
    void Callback_pin_0xC5(void);
    extern types::platforms::arduino::Pin pin_0xC6;
    void Callback_pin_0xC6(void);
    extern types::platforms::arduino::Pin pin_0xC7;
    void Callback_pin_0xC7(void);
    extern types::platforms::arduino::Pin pin_0xC8;
    void Callback_pin_0xC8(void);
    extern types::platforms::arduino::Pin pin_0xC9;
    void Callback_pin_0xC9(void);


    /**************************************************************************
     *                  SPI
     *************************************************************************/
    extern types::platforms::arduino::SPI spi_1;

}
