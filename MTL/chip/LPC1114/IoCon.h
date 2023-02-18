//------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
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

// \brief NXP LPC1114 I/O Configuration
//
// Data source NXP document "LPC111x-User Manual UM10398"

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union IoConReg
{
   REG_ARRAY(0x000, pio, 54);
};

class IoCon : public Periph<IoConReg,0x40044000>
{
public:
   enum Mode
   {
      PULL_NONE = 0,
      PULL_DOWN = 1,
      PULL_UP   = 2,
      REPEAT    = 3,
   };

   void config(unsigned pin,
               unsigned func,
               Mode     mode,
               bool     hys = false,
               bool     inv = false,
               bool     od  = false)
   {
      unsigned port  = pin >> 5;
      unsigned bit   = pin & 0x1F;
      unsigned index = port * 24 + bit;

      uint32_t data = reg->pio[index];

      data = (data & 0xFFFFFB80) |
             (func <<  0) |
             (mode <<  3) |
             (hys  <<  5) |
             (inv  <<  6) |
             (od   << 10);

      reg->pio[index] = data;
   }
};

} // namespace MTL
