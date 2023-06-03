//------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
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

// \brief RP2040 XOSC peripheral

#pragma once

#include "MTL/Periph.h"

namespace MTL {

struct XOscReg
{
   uint32_t ctrl;
   uint32_t status;
   uint32_t dormant;
   uint32_t startup;
   uint32_t pad[3];
   uint32_t count;
};

class XOsc : public Periph<XOscReg, 0x40024000>
{
public:
   //! Start the XTAL oscillator
   void start()
   {
      reg->ctrl    = CTRL_FREQ_RANGE_1_15MHZ;
      reg->startup = ((XTAL_FREQ_MHZ * 1000) + 128) >> 8;

      // enable
      reg->ctrl    = (0xFAB << 12) | CTRL_FREQ_RANGE_1_15MHZ;

      // Wait until stable
      while((reg->status & (1<<12)) == 0);
   }

   static const unsigned XTAL_FREQ_MHZ           = 12;
   static const uint32_t CTRL_FREQ_RANGE_1_15MHZ = 0xAA0;
};

} // namespace MTL
