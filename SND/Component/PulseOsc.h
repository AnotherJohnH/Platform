//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Osc.h"

namespace SND {

class PulseOsc : public Osc
{
public:
   ControlIn<Signal> width;

   PulseOsc(Freq freq_hz = 0.0)
      : Osc(freq_hz)
      , width(this)
   {}

private:
   unsigned phase_width{PHASE_PERIOD / 2};

   void setWidth(Signal width_)
   {
      phase_width = PHASE_PERIOD * width_ / 2;
   }

   virtual void controlEvent(Control* control) override
   {
      if(control == &width)
      {
         setWidth(width);
      }
      else
      {
         Osc::controlEvent(control);
      }
   }

   virtual Signal output() override
   {
      return nextPhase() > phase_width ? -1.0 : +1.0;
   }
};

} // namespace SND

