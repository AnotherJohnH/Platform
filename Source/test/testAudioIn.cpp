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
#include <cstdio>

#include "PLT/Audio.h"


static int16_t data[1024];


class TestAudioIn : public PLT::Audio::In
{
public:
   TestAudioIn() : PLT::Audio::In(44100, PLT::Audio::SINT16, 1) {}

   virtual void setSamples(const int16_t* buffer, unsigned n) override
   {
      for(unsigned i=0; i<n; ++i)
      {
         data[i] = buffer[i];
      }
   }
};


class TestAudioOut : public PLT::Audio::Out
{
public:
   TestAudioOut() : PLT::Audio::Out(44100, PLT::Audio::SINT16, 1) {}

   virtual void getSamples(int16_t* buffer, unsigned n) override
   {
      for(unsigned i=0; i<n; ++i)
      {
         buffer[i] = data[i];
      }
   }
};


int main()
{
   TestAudioIn   audio_in;
   TestAudioOut  audio_out;

   audio_in.setEnable(true);
   audio_out.setEnable(true);

   getchar();
}

