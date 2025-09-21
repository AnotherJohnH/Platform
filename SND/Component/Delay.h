//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef SND_DELAY_H
#define SND_DELAY_H

#include <vector>

#include "SND/Control.h"
#include "SND/SignalOp.h"

namespace SND {


//! Delay line
class Delay : public UnaryOp, public Control::Observer
{
public:
   ControlIn<unsigned> control;

   Delay(unsigned initial = 1)
      : control(this)
   {
      control = initial;
   }

private:
   std::vector<Signal> buffer;
   unsigned            index{0};

   virtual void controlEvent(Control*) override
   {
      if(control == 0)
      {
         control = 1;
      }

      buffer.resize(control);

      for(auto& signal : buffer)
      {
         signal = 0.0;
      }
   }

   virtual Signal output() override
   {
      Signal out    = buffer[index];
      buffer[index] = in;

      if(++index >= buffer.size())
      {
         index = 0;
      }

      return out;
   }
};


} // namespace SND

#endif
