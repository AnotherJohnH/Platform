//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief picoX21H hardware configurations

#pragma once

#if not defined(HW_NATIVE)
#include "MTL/chip/Config.h"
#endif

//------------------------------------------------------------------------------
#if defined(HW_RPIPICO_I2S_DAC)

#include "MTL/chip/module/RPiPico.h"

#define HW_DESCR          "I2S DAC"

#define HW_USB_DEVICE
#define HW_LED            MTL::PIN_LED1

#define HW_LCD_I2C        MTL::I2C1_P24_P25
#define HW_LCD_I2C_ADDR   0x3E

#define HW_MIDI_IN_UART   MTL::Uart1_P26_P27

#define HW_DAC_I2S_GENERIC
#define HW_DAC_I2S_SD     MTL::PIN_31
#define HW_DAC_I2S_CLKS   MTL::PIN_32

#define HW_YM2151
#define HW_YM2151_CTRL4   MTL::PIN_4
#define HW_YM2151_CLK     MTL::PIN_9
#define HW_YM2151_DAC_IN  MTL::PIN_10
#define HW_YM2151_DATA8   MTL::PIN_14

//------------------------------------------------------------------------------
#elif defined(HW_RPIPICO_PWM_DAC)

#include "MTL/chip/module/RPiPico.h"

#define HW_DESCR          "PWM DAC"

#define HW_USB_DEVICE
#define HW_LED            MTL::PIN_LED1

#define HW_LCD_I2C        MTL::I2C1_P24_P25
#define HW_LCD_I2C_ADDR   0x3E

#define HW_MIDI_IN_UART   MTL::Uart1_P26_P27

#define HW_DAC_PWM        MTL::PIN_31

#define HW_YM2151
#define HW_YM2151_CTRL4   MTL::PIN_4
#define HW_YM2151_CLK     MTL::PIN_9
#define HW_YM2151_DAC_IN  MTL::PIN_10
#define HW_YM2151_DATA8   MTL::PIN_14

//------------------------------------------------------------------------------
#elif defined(HW_RPIPICO_WAVESHARE_REV2_1)

#include "MTL/chip/module/RPiPico.h"

#define HW_DESCR          "WaveShare Rev2.1 I2S DAC (piggy-back)"

#define HW_USB_DEVICE
#define HW_LED            MTL::PIN_LED1

#define HW_MIDI_IN_UART   MTL::Uart1_P26_P27

#define HW_LCD_I2C        MTL::I2C1_P24_P25
#define HW_LCD_I2C_ADDR   0x3E

#define HW_LED_7_SEG
#define HW_LED_7_SEG_DAT  MTL::PIN_21
#define HW_LED_7_SEG_CLK  MTL::PIN_22

#define HW_DAC_I2S_WAVESHARE_REV2_1
#define HW_DAC_I2S_SD     MTL::PIN_29
#define HW_DAC_I2S_CLKS   MTL::PIN_32
#define HW_DAC_I2S_MCLK   MTL::PIN_31

#define HW_YM2151
#define HW_YM2151_CTRL4   MTL::PIN_4
#define HW_YM2151_CLK     MTL::PIN_9
#define HW_YM2151_DAC_IN  MTL::PIN_10
#define HW_YM2151_DATA8   MTL::PIN_14

//------------------------------------------------------------------------------
#elif defined(HW_NATIVE)

#define HW_DESCR          "native"

#define HW_USB_FILE_NATIVE
#define HW_LED_NATIVE
#define HW_MIDI_IN_NATIVE
#define HW_LCD_NATIVE
#define HW_DAC_NATIVE
#define HW_LED_7_SEG_NATIVE

//------------------------------------------------------------------------------
#else

#error "Target hardware not specified"

#endif

//==============================================================================

#include "Hardware/Device/Dac.h"
#include "Hardware/Device/PhysMidi.h"
#include "Hardware/Device/Lcd.h"
#include "Hardware/Device/Led.h"
#include "Hardware/Device/UsbFileMidi.h"
#include "Hardware/Device/YM2151.h"
