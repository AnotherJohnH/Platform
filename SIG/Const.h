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

} // namespace SIG
