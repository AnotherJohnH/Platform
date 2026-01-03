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
#define HW_LED   MTL::PIN_LED1

//------------------------------------------------------------------------------
#elif defined(HW_BADGER2040)

#include "MTL/chip/module/badger2040.h"

#define HW_DESCR "badger2040"
#define HW_LED   MTL::badger2040::PIN_USER_LED

//------------------------------------------------------------------------------
#elif defined(HW_TUFTY2040)

#include "MTL/chip/module/tufty2040.h"

#define HW_DESCR "tufty2040"
#define HW_LED   MTL::tufty2040::PIN_USER_LED

//------------------------------------------------------------------------------
#elif defined(HW_MBED)

#include "MTL/chip/module/mbed.h"

#define HW_DESCR "mbed"
#define HW_LED   MTL::PIN_LED1

//------------------------------------------------------------------------------
#elif defined(HW_MICROBIT)

#include "MTL/chip/module/microbit.h"

#define HW_DESCR "microbit"
#define HW_LED   MTL::PIN_LED1

//------------------------------------------------------------------------------
#elif defined(HW_NATIVE)

#define HW_DESCR          "native"
#define HW_LED_NATIVE

//------------------------------------------------------------------------------
#else

#error "Target hardware not specified"

#endif

//==============================================================================

#include "Hardware/Device/Led.h"
