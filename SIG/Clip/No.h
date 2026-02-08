//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/Types.h"

namespace SIG {

namespace Clip {

//! No clip pass thru
class No
{
public:
   inline Signal operator()(Signal x_)
   {
      return x_;
   }
};

} // namespace Clip

} // namespace SIG
