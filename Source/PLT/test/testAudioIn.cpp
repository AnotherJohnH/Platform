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


#include <cstdint>

#include "PLT/Audio.h"
#include "PLT/Event.h"


static const unsigned           SAMPLE_FREQUENCY = 44100;
static const PLT::Audio::Format FORMAT           = PLT::Audio::Format::SINT16;
static const unsigned           CHANNELS         = 1;


// TODO this just happens to match the SDL2 implementation
//      need a more portable way of doing this
static const unsigned BLOCK_SAMPLES    = 1024;


static int16_t data[BLOCK_SAMPLES];


class TestAudioIn : public PLT::Audio::In
{
public:
   TestAudioIn()
      : PLT::Audio::In(SAMPLE_FREQUENCY, FORMAT, CHANNELS)
   {}

   virtual void setSamples(const int16_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; ++i)
      {
         data[i] = buffer[i];
      }
   }
};


class TestAudioOut : public PLT::Audio::Out
{
public:
   TestAudioOut()
      : PLT::Audio::Out(SAMPLE_FREQUENCY, FORMAT, CHANNELS)
   {}

   virtual void getSamples(int16_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; ++i)
      {
         buffer[i] = data[i];
      }
   }
};


int main()
{
   TestAudioIn  audio_in;
   TestAudioOut audio_out;

   audio_in.setEnable(true);
   audio_out.setEnable(true);

   return PLT::Event::mainLoop();
}
