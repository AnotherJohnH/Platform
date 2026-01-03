//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Temperature sensor interface

#pragma once

#if defined(HW_RTC)

#include "MTL/rp2040/Rtc.h"

#elif defined(HW_RTC_NATIVE)

#include "native/Rtc.h"

#endif

namespace hw {

#if defined(HW_RTC)

using Rtc = MTL::Rtc;

#elif defined(HW_RTC_NATIVE)

using Rtc = Rtc<7*24*60>;

#endif

} // namespace hw
