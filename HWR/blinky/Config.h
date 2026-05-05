//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief blinky hardware configurations

#pragma once

#if not defined(HW_NATIVE)
#include "MTL/chip/Config.h"
#endif

//------------------------------------------------------------------------------
#if defined(HW_RPIPICO)

#include "MTL/chip/module/RPiPico.h"

#define HW_DESCR "RPiPico"

#define HW_USB_FILE_DEVICE
#define HW_LED   MTL::PIN_LED1

//------------------------------------------------------------------------------
#elif defined(HW_BADGER2040)

#include "MTL/chip/module/badger2040.h"

#define HW_DESCR "badger2040"

#define HW_USB_FILE_DEVICE
#define HW_LED   MTL::badger2040::PIN_USER_LED

//------------------------------------------------------------------------------
#elif defined(HW_TUFTY2040)

#include "MTL/chip/module/tufty2040.h"

#define HW_DESCR "tufty2040"

#define HW_USB_FILE_DEVICE
#define HW_LED   MTL::tufty2040::PIN_USER_LED

//------------------------------------------------------------------------------
#elif defined(HW_TINY2350)

#include "MTL/chip/module/tiny2350.h"

#define HW_DESCR "tiny2350"

#define HW_USB_FILE_DEVICE
#define HW_LED   MTL::PIN_LED1

//------------------------------------------------------------------------------
#elif defined(HW_MBED)

#include "MTL/chip/module/mbed.h"

#define HW_DESCR "mbed"
#define HW_LED   MTL::PIN_LED1

//------------------------------------------------------------------------------
#elif defined(HW_DIP)

#include "MTL/chip/Pins.h"

#define HW_DESCR "dip"
#define HW_LED   MTL::PIN_LED

//------------------------------------------------------------------------------
#elif defined(HW_MICROBIT)

#include "MTL/chip/module/microbit.h"

#define HW_DESCR "microbit"
#define HW_LED   MTL::PIN_LED1

//------------------------------------------------------------------------------
#elif defined(HW_NATIVE)

#define HW_DESCR          "native"
#define HW_USB_FILE_NATIVE
#define HW_LED_NATIVE

//------------------------------------------------------------------------------
#else

#error "Target hardware not specified"

#endif

//==============================================================================

#include "HWR/Device/Led.h"
#include "HWR/Device/UsbFile.h"
