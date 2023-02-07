//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#ifndef SND_PULSE_OSC_H
#define SND_PULSE_OSC_H

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

#endif
