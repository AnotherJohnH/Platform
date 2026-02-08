//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/Types.h"

namespace SIG {

namespace Clip {

//! Hard clip signal to -1.0...+1.0
class Hard
{
public:
   inline Signal operator()(Signal x_)
   {
           if (x_ > +1.0f) return +1.0f;
      else if (x_ < -1.0f) return -1.0f;

      return x_;
   }
};

} // namespace Clip

} // namespace SIG
