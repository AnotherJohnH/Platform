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

#ifndef SND_MONITOR_H
#define SND_MONITOR_H

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

#endif
