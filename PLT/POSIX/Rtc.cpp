//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Stub RTC implementation

#include <ctime>
#include <sys/time.h>

#include "PLT/Rtc.h"

namespace PLT {

namespace Rtc {

bool getDateAndTime(DateAndTime& value)
{
   time_t now = time(nullptr);
   if (now == -1) return false;

   struct tm* info = localtime(&now);
   value.year   = 1900 + info->tm_year;
   value.month  = info->tm_mon;
   value.day    = info->tm_mday;
   value.hour   = info->tm_hour;
   value.minute = info->tm_min;
   value.second = info->tm_sec;

   return true;
}

bool getEpocTime(Time& value)
{
   struct timeval now;
   if (gettimeofday(&now, nullptr) == -1) return false;

   value.second       = now.tv_sec;
   value.micro_second = now.tv_usec;

   return true;
}

} // namespace Rtc

} // namespace PLT
