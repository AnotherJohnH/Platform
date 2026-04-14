//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cstdint>

#if defined(SIG_FX32)
#include "STB/FixP.h"
#include "STB/UFixP.h"
#endif

namespace SIG {

#if defined(SIG_FL64)
using Float  = double;
using Signal = double;
using Phase  = double;
using Freq   = double;
#elif defined(SIG_FL32)
using Float  = float;
using Signal = float;
using Phase  = float;
using Freq   = float;
#elif defined(SIG_FX32)
using Float  = STB::FixP<16>;
using Signal = STB::FixP<16>;
using Phase  = STB::FixP<24>;
using Freq   = STB::UFixP<16>;
#else
#error "Float type must be defined e.g. -DSIG_FL32"
#endif

using UPhase = uint32_t;
using SPhase = int32_t;
using Rate   = unsigned;

} // namespace SIG
