//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "PLT/Audio.h"
#include "SND/Signal.h"


namespace SND {


//! Basic monophonic audio sink
class Monitor : public PLT::Audio::Out
{
public:
   SignalIn             in;
   ControlOut<uint32_t> ticks;

   Monitor(Freq tick_freq = 1000.0, Freq sample_freq = 44100.0)
      : PLT::Audio::Out(sample_freq, PLT::Audio::Format::SINT16, /* channels */ 1)
      , sink(sample_freq)
      , tick_period(sample_freq / tick_freq)
   {}

   Freq getSampleFreq() const { return sink.sample_freq; }

   void play()
   {
      in.init(&sink);

      setEnable(true);
   }

   void stop() { setEnable(false); }

protected:
   virtual void getSamples(int16_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; ++i)
      {
         if(--tick_count == 0)
         {
            tick_count = tick_period;
            ticks      = ticks + 1;
         }

         buffer[i] = in * 0x7FFF;
         sink.sequence++;
      }
   }

private:
   SignalSink sink;
   uint32_t   tick_period{};
   uint32_t   tick_count{1};
};


} // namespace SND

