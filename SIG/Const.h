//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Types.h"

namespace SIG {

static const SPhase SPHASE_MAX{0x7FFFFFFF};
static const UPhase UPHASE_MAX{0xFFFFFFFF};

#if defined(SIG_SAMPLE_RATE)
static const Rate SAMPLE_RATE{SIG_SAMPLE_RATE};
#else
static const Rate SAMPLE_RATE{48000};   //!< 48.0 kHz
#endif

constexpr Float GAIN_MAX_DB = +20.0f;
constexpr Float GAIN_MIN_DB = -60.0f;

constexpr signed GAIN15_60_DB = 0x6000;
constexpr signed GAIN15_40_DB = 0x4000;
constexpr signed GAIN15_20_DB = 0x2000;
constexpr signed GAIN15_0_DB  = 0x0000;

} // namespace SIG
