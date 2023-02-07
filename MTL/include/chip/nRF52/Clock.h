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
//
// NORDIC Semiconductor
// nRF52
// Clock control
//

#ifndef MTL_NRF52_CLOCK_H
#define MTL_NRF52_CLOCK_H

#include "MTL/Periph.h"

namespace MTL {

namespace nRF52 {

union ClockReg
{
   REG(0x000, task_hfclkstart);
   REG(0x004, task_hfclkstop);
   REG(0x008, task_lfclkstart);
   REG(0x00C, task_lfclkstop);
   REG(0x010, task_cal);
   REG(0x014, task_ctstart);
   REG(0x018, task_ctstop);
   REG(0x100, events_hfclkstarted);
   REG(0x104, events_lfclkstarted);
   REG(0x10C, events_done);
   REG(0x110, events_ctto);
   REG(0x128, events_ctstarted);
   REG(0x12C, events_ctstopped);
   REG(0x304, intenset);
   REG(0x308, intenclr);
   REG(0x408, hfclkrun);
   REG(0x40C, hfclkstat);
   REG(0x414, lfclkrun);
   REG(0x418, lfclkstat);
   REG(0x41C, lfclksrccopy);
   REG(0x518, lfclksrc);
   REG(0x528, hfxodebounce);
   REG(0x52C, lfxodebounce);
   REG(0x538, ctiv);
   REG(0x55c, traceconfig);
};

class Clock : public Periph<ClockReg,0x40000000>
{
public:
   Clock() = default;

   //! Start the HF crystal clock and wait until start complete
   void startHFXO()
   {
      reg->task_hfclkstart = 1;

      while(reg->events_hfclkstarted == 0);
   }

private:
   using Periph<ClockReg,0x40000000>::reg;
};

} // namespace nRF52

} // namespace MTL

#endif
