//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2040 Timer peripheral

#pragma once

#include "MTL/Periph.h"

namespace MTL {

struct TimerReg
{
   uint32_t timehw;
   uint32_t timelw;
   uint32_t timehr;
   uint32_t timelr;
   uint32_t alarm0;
   uint32_t alarm1;
   uint32_t alarm2;
   uint32_t alarm3;
   uint32_t armed;
   uint32_t timeawh;
   uint32_t timeawl;
   uint32_t dbgpause;
   uint32_t pause;
   uint32_t intr;
   uint32_t inte;
   uint32_t intf;
   uint32_t ints;
};

//! 1 MHz timer
class Timer
   : public Periph<TimerReg, 0x40054000>
{
public:
   Timer() = default;

   //! Get 64-bit timer value XXX not thread safe
   uint64_t read64() const
   {
      uint32_t lsw = reg->timelr;
      uint32_t msw = reg->timehr;

      return (uint64_t(msw) << 32) | lsw;
   }

   //! Get 32-bit timer value
   uint32_t read32() const
   {
      return reg->timeawl;
   }
};

} // namespace MTL
