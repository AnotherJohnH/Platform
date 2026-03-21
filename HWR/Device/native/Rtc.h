//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdlib>

#include "PLT/Event.h"

extern "C" void IRQ_RTC();

template <unsigned IRQ_LIMIT = 0>
class Rtc
{
public:
   Rtc() = default;

   unsigned getDOTW()   const { return dow; }
   unsigned getDay()    const { return day; }
   unsigned getHour()   const { return hour; }
   unsigned getMinute() const { return minute; }

   void setDate(unsigned year_, unsigned month_, unsigned day_)
   {
      year  = year_;
      month = month_;
      day   = day_;
   }

   void setTime(unsigned hour_, unsigned minute_, unsigned second_, unsigned dow_)
   {
      dow    = dow_;
      hour   = hour_;
      minute = minute_;
      second = second_;
   }

   void start() {}

   void sleep(unsigned seconds_)
   {
      IRQ_RTC();

      second += seconds_;
      if (second < 60) return;

      minute += second / 60;
      second  = second % 60;
      if (minute < 60) return;

      hour  += minute / 60;
      minute = minute % 60;
      if (hour < 24) return;
      
      day   += hour / 24;
      dow    = (dow + (hour / 24)) % 7;
      hour   = hour % 24;
   }

   void irq()
   {
      PLT::Event::Message event;
      PLT::Event::poll(event);

      if (event.type == PLT::Event::QUIT)
         exit(0);

      if (++irq_count == IRQ_LIMIT)
      {
         PLT::Event::eventLoop();
         exit(0);
      }
   }

private:
   unsigned irq_count {0};

   unsigned year;
   unsigned month;
   unsigned day;
   unsigned dow;
   unsigned hour;
   unsigned minute;
   unsigned second;
};
