//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief micro::bit module buttons

#pragma once

#include "MTL/Digital.h"

namespace MTL {

class Reset
{
private:
   Digital::Out<PIN_RESET>  reset{true};

public:
   //! Request a reset
   void go()
   {
      reset = false;
   }
};

} // namespace MTL
