//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef SND_CLAMP_H
#define SND_CLAMP_H

#include "SND/Control.h"
#include "SND/SignalOp.h"

namespace SND {

//! Clamp signal against controllable max and min limits
class Clamp : public UnaryOp
{
public:
   ControlIn<Signal> max;
   ControlIn<Signal> min;

   Clamp(Signal max_ = +1.0, Signal min_ = -1.0)
      : max(max_)
      , min(min_)
   {}

private:
   virtual Signal output() override
   {
      Signal value = in;

      if(value > max)
      {
         value = max;
      }
      else if(value < min)
      {
         value = min;
      }

      return value;
   }
};


} // namespace SND

#endif
