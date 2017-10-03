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

#ifndef SND_MICROPHONE_H
#define SND_MICROPHONE_H

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

   STB::Fifo<Signal,11>  fifo;

   Microphone()
      : In(SAMPLE_FREQ, PLT::Audio::SINT16, /* channels */1)
   {}

private:
   virtual void init(const SignalSink *sink) override
   {
      assert(sink->sample_freq == SAMPLE_FREQ);

      setEnable(true);
   }

   virtual Signal output() override
   {
      if (fifo.empty()) return 0;

      Signal sample = fifo.back();

      fifo.pop();

      return sample;
   }

private:
   virtual void setSamples(const int16_t* buffer, unsigned n) override
   {
      for(unsigned i=0; i<n; ++i)
      {
         if (!fifo.full())
         {
            fifo.push(Signal(buffer[i])/0x8000);
         }
      }
   }
};


} // namespace SND

#endif
