//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file Rtc.h
//! \brief Platform abstraction layer for real-time clock

#ifndef PLT_RTC_H
#define PLT_RTC_H

#include <cstdint>

//! Platform abstraction layer
namespace PLT {

namespace Rtc {

struct DateAndTime
{
   uint16_t year;
   uint8_t  month;
   uint8_t  day;
   uint8_t  hour;
   uint8_t  minute;
   uint8_t  second;
};

struct Time
{
   uint32_t second;
   uint32_t micro_second;
};

//! Get local data and time
bool getDateAndTime(DateAndTime& date_and_time);

//! Get time since GMT 1970 00:00:00
bool getEpocTime(Time& epoc_time);

} // namespace Rtc

} // namespace PLT

#endif // PLT_RTC_H
