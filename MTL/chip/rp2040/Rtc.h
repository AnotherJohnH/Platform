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
#include "MTL/CortexM0/NVIC.h"

#include "Clocks.h"
#include "unistd.h"

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

      setDate(1970, 0, 0);
      setTime(0, 0, 0);

      start();
   }

   bool isRunning() const { return (reg->ctrl        & (1 <<  1)) != 0; }
   bool isMatch()   const { return (reg->irq_setup_0 & (1 << 29)) != 0; }

   //! Set the current date
   void setDate(unsigned year, unsigned month, unsigned day)
   {
      stop();

      reg->setup_0 = (year << 12) | (month << 8) | day;
   }

   //! Set the current time
   void setTime(unsigned hour, unsigned minute, unsigned second, unsigned dotw = 0)
   {
      stop();

      reg->setup_1 = (dotw << 24) | (hour << 16) | (minute << 8) | second;
   }

   //! Set an alarm to occur at a specific time
   void sleepUntil(unsigned hour, unsigned minute, unsigned second)
   {
      // XXX not yet clear why this is required
      usleep(100);

      clrAlarm();

      reg->irq_setup_1 = (1 << 30) | (hour   << 16) |
                         (1 << 29) | (minute <<  8) |
                         (1 << 28) | (second);

      reg->inte = 1;

      MTL::NVIC<25>().enable();

      waitForAlarm();
   }

   //! Set an alarm to occur after a fixed amount of time
   //! Start time is at the last call to sample()
   void sleepFor(unsigned hours, unsigned minutes, unsigned seconds)
   {
      if (first_alarm_after_start)
      {
         // XXX not yet clear why this is required
         seconds += 1;

         first_alarm_after_start = false;
      }

      unsigned hour   = getHour()   + hours;
      unsigned minute = getMinute() + minutes;
      unsigned second = getSecond() + seconds;

      if (second >= 60)
      {
         second -= 60;
         minute++;
      }

      if (minute >= 60)
      {
         minute -= 60;
         hour++;
      }

      if (hour >= 24)
      {
         hour -= 24;
      }

      sleepUntil(hour, minute, second);
   }

   //! Set an alarm to occur after a fixed amount of time
   //! Start time is at the last call to sample()
   void sleep(unsigned seconds)
   {
      unsigned minutes = seconds / 60;
      seconds          = seconds % 60;
      unsigned hours   = minutes / 60;
      minutes          = minutes % 60;

      sleepFor(hours, minutes, seconds);
   }

   void irq()
   {
      clrAlarm();

      sample();
   }

   void clrAlarm()
   {
      reg->inte        = 0;
      reg->irq_setup_0 = 0;
   }

   //! Start the clock
   void start()
   {
      stop();

      reg->ctrl = 1 << 4; // Load
      reg->ctrl = 1 << 0; // enable

      while(not isRunning());

      // Wait for first clock refresh
      const uint32_t rtc_0 = reg->rtc_0;
      while(rtc_0 == reg->rtc_0);

      sample();

      first_alarm_after_start = true;
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
   void waitForAlarm()
   {
      // Enable the alarm
      reg->irq_setup_0 = 1 << 28;

      while(not isMatch())
      {
         __asm("wfi");
      }
   }

   uint32_t rtc_0{};
   uint32_t rtc_1{};
   bool     first_alarm_after_start{};
};

} // namespace MTL
