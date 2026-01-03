//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief picoX7 hardware configurations

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

#define HW_LED_7_SEG
#define HW_LED_7_SEG_DAT  MTL::PIN_21
#define HW_LED_7_SEG_CLK  MTL::PIN_22

#define HW_BUTTONS        1
#define HW_BUTTONS_PIN    MTL::PIN_29

#define HW_PROFILER

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

#define HW_LED_7_SEG
#define HW_LED_7_SEG_DAT  MTL::PIN_21
#define HW_LED_7_SEG_CLK  MTL::PIN_22

#define HW_BUTTONS        1
#define HW_BUTTONS_PIN    MTL::PIN_29

#define HW_PROFILER

//------------------------------------------------------------------------------
#elif defined(HW_RPIPICO_WAVESHARE_I2S_DAC_REV2_1)

#include "MTL/chip/module/RPiPico.h"

#define HW_DESCR          "WaveShare Rev2.1 I2S DAC (piggy-back)"

#define HW_USB_DEVICE
#define HW_LED            MTL::PIN_LED1

#define HW_MIDI_IN_UART   MTL::Uart1_P26_P27

#define HW_LCD_I2C        MTL::I2C1_P24_P25
#define HW_LCD_I2C_ADDR   0x3E

#define HW_DAC_I2S_WAVESHARE_REV2_1
#define HW_DAC_I2S_SD     MTL::PIN_29
#define HW_DAC_I2S_CLKS   MTL::PIN_32
#define HW_DAC_I2S_MCLK   MTL::PIN_31

#define HW_LED_7_SEG
#define HW_LED_7_SEG_DAT  MTL::PIN_21
#define HW_LED_7_SEG_CLK  MTL::PIN_22

#define HW_PROFILER

//------------------------------------------------------------------------------
#elif defined(HW_RPIPICO_PIMORONI_PICO_AUDIO)

#include "MTL/chip/module/RPiPico.h"

#define HW_DESCR          "Pimoroni pico audio I2S DAC (piggy-back)"

#define HW_USB_DEVICE
#define HW_LED            MTL::PIN_LED1

#define HW_MIDI_IN_UART   MTL::Uart1_P26_P27

#define HW_LCD_I2C        MTL::I2C1_P24_P25
#define HW_LCD_I2C_ADDR   0x3E

#define HW_DAC_I2S_GENERIC
#define HW_DAC_I2S_SD     MTL::PIN_12
#define HW_DAC_I2S_CLKS   MTL::PIN_14

#define HW_LED_7_SEG
#define HW_LED_7_SEG_DAT  MTL::PIN_21
#define HW_LED_7_SEG_CLK  MTL::PIN_22

#define HW_BUTTONS        1
#define HW_BUTTONS_PIN    MTL::PIN_29

#define HW_PROFILER

//------------------------------------------------------------------------------
#elif defined(HW_PIMORONI_VGA_DEMO)

#define HW_DESCR          "Pimoroni VGA Demo I2S DAC"

#include "MTL/rp2040/Pins.h"

#define HW_USB_DEVICE
#define HW_LED            MTL::IO_PIN_25

#define HW_MIDI_IN_FAKE

#define HW_DAC_I2S_GENERIC
#define HW_DAC_I2S_SD     MTL::IO_PIN_26
#define HW_DAC_I2S_CLKS   MTL::IO_PIN_27

#define HW_PROFILER

//------------------------------------------------------------------------------
#elif defined(HW_NATIVE)

#define HW_DESCR          "native"

#define HW_USB_FILE_NATIVE
#define HW_LED_NATIVE
#define HW_MIDI_IN_NATIVE
#define HW_LCD_NATIVE
#define HW_DAC_NATIVE
#define HW_LED_7_SEG_NATIVE
#define HW_BUTTONS        1

//------------------------------------------------------------------------------
#else

#error "Target hardware not specified"

#endif

//==============================================================================

#include "Hardware/Device/Audio.h"
#include "Hardware/Device/Buttons.h"
#include "Hardware/Device/PhysMidi.h"
#include "Hardware/Device/Lcd.h"
#include "Hardware/Device/Led.h"
#include "Hardware/Device/UsbFileMidi.h"
#include "Hardware/Device/Led7Seg.h"
#include "Hardware/Device/Profiler.h"
