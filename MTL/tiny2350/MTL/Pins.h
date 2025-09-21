//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief module pins

#pragma once

#include "MTL/chip/Pins.h"

namespace MTL {

static const unsigned PIN_4    = IO_PIN_29;
static const unsigned PIN_5    = IO_PIN_28;
static const unsigned PIN_6    = IO_PIN_27;
static const unsigned PIN_7    = IO_PIN_26;

static const unsigned PIN_9    = IO_PIN_7;
static const unsigned PIN_10   = IO_PIN_6;
static const unsigned PIN_11   = IO_PIN_5;
static const unsigned PIN_12   = IO_PIN_4;
static const unsigned PIN_13   = IO_PIN_3;
static const unsigned PIN_14   = IO_PIN_2;
static const unsigned PIN_15   = IO_PIN_1;
static const unsigned PIN_16   = IO_PIN_0;

static const unsigned PIN_LED1 = IO_PIN_18;
static const unsigned PIN_LED2 = IO_PIN_19;
static const unsigned PIN_LED3 = IO_PIN_20;

static const unsigned PIN_LED_RED = PIN_LED1;
static const unsigned PIN_LED_GRN = PIN_LED2;
static const unsigned PIN_LED_BLU = PIN_LED2;

static const unsigned PIN_IGNORE = 0xFFFFFFFF;

} // namespace MTL
