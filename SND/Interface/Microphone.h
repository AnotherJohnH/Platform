//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cassert>

#include "PLT/Audio.h"
#include "STB/Fifo.h"

#include "SND/Signal.h"


namespace SND {


class Microphone : public PLT::Audio::In
                 , public SignalOut
{
public:
   static const unsigned SAMPLE_FREQ = 44100;

   STB::Fifo<Signal, 11> fifo;

   Microphone()
      : In(SAMPLE_FREQ, PLT::Audio::Format::SINT16, /* channels */ 1)
   {}

private:
   virtual void init(const SignalSink* sink) override
   {
      assert(sink->sample_freq == SAMPLE_FREQ);

      setEnable(true);
   }

   virtual Signal output() override
   {
      if(fifo.empty()) return 0;

      Signal sample = fifo.back();

      fifo.pop();

      return sample;
   }

private:
   virtual void setSamples(const int16_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; ++i)
      {
         if(!fifo.full())
         {
            fifo.push(Signal(buffer[i]) / 0x8000);
         }
      }
   }
};


} // namespace SND

