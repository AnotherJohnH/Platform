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

// \brief NXP LPC1768 DAC
//
// Data source NXP document "LPC17xx User Manual UM10360"

#pragma once

#include "MTL/Periph.h"

#include "SysCon.h"
#include "PinCon.h"

namespace MTL {

union DACReg
{
   REG(0x000, dacr);      //!< D/A converter
   REG(0x004, dacctrl);   //!< DAC control
   REG(0x008, daccntval); //!< DAC counter value
};

class DAC : public Periph<DACReg,0x4008C000>
{
public:
   DAC(unsigned freq, bool bias_ = true)
   {
      SysCon syscon;
      PinCon pincon;

      syscon.pclksel0(PCLK_DAC, 1);

      pincon.config(PIN_0_26, 0b10, PinCon::PULL_NONE);

      if (bias_)
          bias = 1<<16;

      reg->daccntval = CLOCK_FREQ / freq;

      // CNT_ENA | DBLBUF_ENA
      reg->dacctrl = 0b110;

      send(0);
   }

   void send(int16_t value)
   {
      reg->dacr = bias | (value + 0x8000);
   }

private:
   static const unsigned PCLK_DAC = 22;

   uint32_t bias {0};
};

} // namespace MTL
