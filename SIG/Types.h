//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cstdint>

namespace SIG {

#if defined(SIG_FP64)
using Float = double;
#else
using Float = float;
#endif

using Signal = Float;
using Phase  = Float;
using Freq   = Float;

using UPhase = uint32_t;
using SPhase = int32_t;
using Rate   = unsigned;

} // namespace SIG
