//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Stub RTC implementation

#include <sys/time.h>

#include "PLT/Rtc.h"

namespace PLT {

namespace Rtc {

bool getDateAndTime(DateAndTime& value)
{
   return false;
}

bool getEpocTime(Time& value)
{
   return false;
}

} // namespace Rtc

} // namespace PLT
