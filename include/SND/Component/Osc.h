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

#ifndef SND_OSC_H
#define SND_OSC_H

#include "SND/Signal.h"

namespace SND {

using Freq = double;

class Osc : public SignalOut
          , public Control::Observer
{
public:
   ControlIn<bool>   sync;
   ControlIn<Freq>   freq;

protected:
   using Phase  = uint32_t;
   static const unsigned LOG2_PHASE_PERIOD = 10;
   static const unsigned PHASE_PERIOD      = 1<<LOG2_PHASE_PERIOD;
   static const unsigned PHASE_FRAC_BITS   = sizeof(Phase)*8 - LOG2_PHASE_PERIOD;

   Osc(Freq freq_)
      : freq(this)
   {
      freq = freq_;
   }

   //! Get the next phase angle of the oscilator
   Phase nextPhase()
   {
      Phase phase = accum >> PHASE_FRAC_BITS;
      accum += delta;
      return phase;
   }

   virtual void controlEvent(Control* control) override
   {
      if (control == &sync)
      {
         if (sync) accum = 0;
      }
      else
      {
         setFreq(freq);
      }
   }

private:
   unsigned  sample_freq{0};
   Phase     accum{0};
   Phase     delta{1<<PHASE_FRAC_BITS};

   //! Set the frequency
   void setFreq(Freq freq_hz)
   {
      delta = freq_hz * PHASE_PERIOD * (1<<PHASE_FRAC_BITS) / sample_freq;
   }

   virtual void init(const SignalSink* sink) override
   {
      sample_freq = sink->sample_freq;

      freq = Freq(freq);
   }
};

} // namespace SND

#endif
