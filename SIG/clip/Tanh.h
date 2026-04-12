//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cmath>

#include "SIG/Types.h"

namespace SIG {

namespace clip {

//! Tanh clip signal to -1.0...+1.0
class Tanh
{
public:
   inline Signal operator()(Signal x, Float drive = Float(3.0))
   {
#if defined(SIG_FL32)
      return tanhf(x * drive);
#elif defined(SIG_FL64)
      return tanh(x * drive);
#endif
   }
};

} // namespace clip

} // namespace SIG
