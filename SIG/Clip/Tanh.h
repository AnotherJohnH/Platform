//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cmath>

#include "SIG/Types.h"

namespace SIG {

namespace Clip {

//! Tanh clip signal to -1.0...+1.0
class Tanh
{
public:
   inline Signal operator()(Signal x, Float drive = 3.0f)
   {
      return tanhf(x * drive);
   }
};

} // namespace Clip

} // namespace SIG
