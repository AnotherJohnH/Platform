//------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
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
