//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Manage a USB end-point

#pragma once

#include "USB/Descr.h"

namespace USB {

class EndPoint
{
public:
   EndPoint() = default;

   EndPoint(Descr& descr_)
   {
      descr_.setImpl(this);
   }
};

} // namespace USB
