//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief module pins

#pragma once

#include "MTL/rp2040/Pins.h"

namespace MTL {

static const unsigned PIN_1    = rp2040::IO_PIN_0;
static const unsigned PIN_2    = rp2040::IO_PIN_1;
static const unsigned PIN_4    = rp2040::IO_PIN_2;
static const unsigned PIN_5    = rp2040::IO_PIN_3;
static const unsigned PIN_6    = rp2040::IO_PIN_4;
static const unsigned PIN_7    = rp2040::IO_PIN_5;
static const unsigned PIN_9    = rp2040::IO_PIN_6;
static const unsigned PIN_10   = rp2040::IO_PIN_7;
static const unsigned PIN_11   = rp2040::IO_PIN_8;
static const unsigned PIN_12   = rp2040::IO_PIN_9;
static const unsigned PIN_14   = rp2040::IO_PIN_10;
static const unsigned PIN_15   = rp2040::IO_PIN_11;
static const unsigned PIN_16   = rp2040::IO_PIN_12;
static const unsigned PIN_17   = rp2040::IO_PIN_13;
static const unsigned PIN_19   = rp2040::IO_PIN_14;
static const unsigned PIN_20   = rp2040::IO_PIN_15;
static const unsigned PIN_21   = rp2040::IO_PIN_16;
static const unsigned PIN_22   = rp2040::IO_PIN_17;
static const unsigned PIN_24   = rp2040::IO_PIN_18;
static const unsigned PIN_25   = rp2040::IO_PIN_19;
static const unsigned PIN_26   = rp2040::IO_PIN_20;
static const unsigned PIN_27   = rp2040::IO_PIN_21;
static const unsigned PIN_29   = rp2040::IO_PIN_22;

static const unsigned PIN_LED1 = rp2040::IO_PIN_25;
static const unsigned PIN_31   = rp2040::IO_PIN_26;
static const unsigned PIN_32   = rp2040::IO_PIN_27;
static const unsigned PIN_34   = rp2040::IO_PIN_28;

static const unsigned PIN_IGNORE = 0xFFFFFFFF;

} // namespace MTL
