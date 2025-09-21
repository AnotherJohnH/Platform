//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef SND_OSC_H
#define SND_OSC_H

#include "SND/Signal.h"

namespace SND {

using Freq = double;

class Osc : public SignalOut, public Control::Observer
{
public:
   ControlIn<bool> sync;
   ControlIn<Freq> freq;

protected:
   using Phase  = uint32_t;

   static const unsigned LOG2_PHASE_PERIOD = 10;
   static const unsigned PHASE_PERIOD      = 1 << LOG2_PHASE_PERIOD;
   static const unsigned PHASE_FRAC_BITS   = sizeof(Phase) * 8 - LOG2_PHASE_PERIOD;

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
      if(control == &sync)
      {
         if(sync) accum = 0;
      }
      else
      {
         setFreq(freq);
      }
   }

private:
   unsigned sample_freq{0};
   Phase    accum{0};
   Phase    delta{1 << PHASE_FRAC_BITS};

   //! Set the frequency
   void setFreq(Freq freq_hz)
   {
      delta = freq_hz * PHASE_PERIOD * (1 << PHASE_FRAC_BITS) / sample_freq;
   }

   virtual void init(const SignalSink* sink) override
   {
      sample_freq = sink->sample_freq;

      freq = Freq(freq);
   }
};

} // namespace SND

#endif
