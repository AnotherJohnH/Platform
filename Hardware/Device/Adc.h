//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief ADC interface

#pragma once

#if defined(HW_ADC)
#include "MTL/chip/Adc.h"
#else
#define HW_ADC_NONE
#endif

namespace hw {

#if defined(HW_ADC)
using Adc = MTL::Adc;
#endif

} // namespace hw
