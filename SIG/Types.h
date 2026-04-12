//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cstdint>

namespace SIG {

#if defined(SIG_FP64)
using Float = double;
#elif defined(SIG_FP32)
using Float = float;
#elif defined(SIG_I32)
using Float = int32_t;
#else
#error "Float type must be defined e.g. -DSIG_FP32"
#endif

using Signal = Float;
using Phase  = Float;
using Freq   = Float;

using UPhase = uint32_t;
using SPhase = int32_t;
using Rate   = unsigned;

} // namespace SIG
