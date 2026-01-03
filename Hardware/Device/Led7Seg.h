//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Hardware 2 digit 7-segment LED display

#pragma once

#if defined(HW_LED_7_SEG)

#include "MTL/Led7Seg.h"

#elif defined(HW_LED_7_SEG_NATIVE)

#include "native/Led7Seg.h"

#endif

namespace hw {

#if defined(HW_LED_7_SEG)

// pico pin 4 : DATA
// pico pin 5 : CLOCK

class Led7Seg : public MTL::Led7Seg<HW_LED_7_SEG_CLK,
                                    HW_LED_7_SEG_DAT,
                                    /* NUM_DIGITS */ 2>
{
public:
   Led7Seg()
   {
      MTL::config.gpio(HW_LED_7_SEG_CLK, ">7-SEG LED clk");
      MTL::config.gpio(HW_LED_7_SEG_DAT, ">7-SEG LED data");
   }
};

#elif defined(HW_LED_7_SEG_NATIVE)

using Led7Seg = ::Led7Seg</* DIGITS */2>;

#else

class Led7Seg
{
public:
   Led7Seg() = default;

   void printDec(unsigned, unsigned) {}
};

#endif

} // namespace hw
