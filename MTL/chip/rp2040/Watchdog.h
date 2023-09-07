//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

// \brief RP2040 Watchdog peripheral

#pragma once

#include "MTL/Periph.h"

namespace MTL {

struct WatchdogReg
{
   uint32_t ctrl;
   uint32_t load;
   uint32_t reason;
   uint32_t scratch[8];
   uint32_t tick;
};

//! 1 MHz timer
class Watchdog
   : public Periph<WatchdogReg, 0x40058000>
{
public:
   Watchdog() = default;

   //! Start the tick clock for a specific reference clock
   void start(unsigned cycles)
   {
      reg->tick = TICK_ENABLE | cycles;
   }

private:
   static const uint32_t TICK_ENABLE = 1 << 9;
};

} // namespace MTL
