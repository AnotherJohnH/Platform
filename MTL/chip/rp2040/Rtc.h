//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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
// \brief RP2040 RTC peripheral #pragma once

#pragma once

#include "MTL/Periph.h"

#include "Clocks.h"

namespace MTL {

struct RtcReg
{
   uint32_t clkdiv_m1;
   uint32_t setup_0;
   uint32_t setup_1;
   uint32_t ctrl;
   uint32_t irq_setup_0;
   uint32_t irq_setup_1;
   uint32_t rtc_1;
   uint32_t rtc_0;
   uint32_t intr;
   uint32_t inte;
   uint32_t intf;
   uint32_t ints;
};

//! Real Time Clock
class Rtc : public Periph<RtcReg, 0x4005C000>
{
public:
   Rtc()
   {
      reg->clkdiv_m1 = Clocks::RTC_FREQ - 1;
   }

   bool isRunning() const { return (reg->ctrl & (1<<1)) != 0; }

   void setDate(unsigned year, unsigned month, unsigned day, unsigned dotw)
   {
      stop();

      reg->setup_0 = (year << 12) | (month << 8) | day;
      reg->setup_1 = (dotw << 24) | (reg->setup_1 & 0xFFFFFF);;
   }

   void setTime(unsigned hour, unsigned minute, unsigned second)
   {
      stop();

      reg->setup_1 = (reg->setup_1 & 0xFF000000) | (hour << 16) | (minute << 8) | second;
   }

   //! Start the clock
   void start()
   {
      reg->ctrl = 1 << 4; // Load
      reg->ctrl = 1 << 0; // enable

      while(not isRunning());

      // Wait for first clock refresh
      while(reg->rtc_0 != reg->setup_1);
   }

   //! Stop the clock
   void stop()
   {
      reg->ctrl = 0;

      while(isRunning());
   }

   //! Sample current data and time
   void sample()
   {
      rtc_0 = reg->rtc_0;
      rtc_1 = reg->rtc_1;
   }

   // Access sampled data and time

   unsigned getYear()   const { return (rtc_1 >> 12) & 0x7FF; }
   unsigned getMonth()  const { return (rtc_1 >>  8) & 0xF; }
   unsigned getDay()    const { return (rtc_1 >>  0) & 0x1F; }
   unsigned getDOTW()   const { return (rtc_0 >> 24) & 0x7; }
   unsigned getHour()   const { return (rtc_0 >> 16) & 0x1F; }
   unsigned getMinute() const { return (rtc_0 >>  8) & 0x3F; }
   unsigned getSecond() const { return (rtc_0 >>  0) & 0x3F; }

private:
   uint32_t rtc_0{};
   uint32_t rtc_1{};
};

} // namespace MTL
