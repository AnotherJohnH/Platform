//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief picoSense hardware configurations

#pragma once

#if not defined(HW_NATIVE)
#include "MTL/chip/Config.h"
#endif

//------------------------------------------------------------------------------
#if defined(HW_RPIPICO_WAVESHARE_EPAPER)

#include "MTL/chip/module/RPiPico.h"

#define HW_DESCR          "WaveShare E-Paper"

#define HW_USB_FILE_DEVICE
#define HW_DISPLAY_WAVESHARE_EPAPER
#define HW_TEMP_SENSE_P21_P22_MCP9808
#define HW_LED               MTL::PIN_LED1
#define HW_RTC
#define HW_BUTTONS_GPIO
#define HW_BUTTONS_GPIO_PIN  MTL::PIN_4

//------------------------------------------------------------------------------
#elif defined(HW_BADGER2040)

#include "MTL/chip/module/badger2040.h"

#define HW_DESCR          "badger2040"

#define HW_USB_FILE_DEVICE
#define HW_DISPLAY_BADGER2040
#define HW_TEMP_SENSE_BADGER_MCP9808
#define HW_LED            MTL::badger2040::PIN_USER_LED
#define HW_RTC
#define HW_BUTTONS_BADGER2040

//------------------------------------------------------------------------------
#elif defined(HW_TUFTY2040)

#include "MTL/chip/module/tufty2040.h"

#define HW_DESCR          "tufty2040"

#define HW_USB_FILE_DEVICE
#define HW_DISPLAY_TUFTY2040
#define HW_TEMP_SENSE_TUFTY_MCP9808
#define HW_LED            MTL::tufty2040::PIN_USER_LED
#define HW_RTC
#define HW_BUTTONS_TUFTY2040

//------------------------------------------------------------------------------
#elif defined(HW_NATIVE)

#define HW_DESCR          "native"

#define HW_USB_FILE_NATIVE
#define HW_DISPLAY_NATIVE
#define HW_DISPLAY_WIDTH 296
#define HW_DISPLAY_HEIGHT 128
#define HW_TEMP_SENSE_NATIVE
#define HW_LED_NATIVE
#define HW_RTC_NATIVE
#define HW_BUTTONS_NATIVE

//------------------------------------------------------------------------------
#else

#error "Target hardware not specified"

#endif

//==============================================================================

#include "Hardware/Device/Display.h"
#include "Hardware/Device/TempSense.h"
#include "Hardware/Device/Led.h"
#include "Hardware/Device/UsbFile.h"
#include "Hardware/Device/Rtc.h"
#include "Hardware/Device/Buttons.h"
