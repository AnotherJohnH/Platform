//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "Types.h"

namespace SIG {

inline constexpr SPhase SPHASE_MAX{0x7FFFFFFF};
inline constexpr UPhase UPHASE_MAX{0xFFFFFFFF};

inline constexpr UPhase UPHASE_QUARTER{0x40000000};
inline constexpr UPhase UPHASE_HALF{UPHASE_QUARTER * 2};

#if defined(SIG_SAMPLE_RATE)
inline constexpr Rate SAMPLE_RATE{SIG_SAMPLE_RATE};
#else
inline constexpr Rate SAMPLE_RATE{48000};   //!< 48.0 kHz
#endif

inline constexpr Float GAIN_MAX_DB = Float{+20.0};
inline constexpr Float GAIN_MIN_DB = Float{-60.0};

inline constexpr signed GAIN15_60_DB = 0x6000;
inline constexpr signed GAIN15_40_DB = 0x4000;
inline constexpr signed GAIN15_20_DB = 0x2000;
inline constexpr signed GAIN15_0_DB  = 0x0000;

} // namespace SIG
