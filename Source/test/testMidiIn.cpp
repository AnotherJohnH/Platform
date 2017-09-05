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


#include <cstdio>

#include "PLT/Midi.h"


class TestMidiIn : public PLT::MIDI::In
{
private:
   virtual void messageIn(unsigned length, uint8_t* data) override
   {
      unsigned i;

      for(i=0; i<length; ++i)
      {
         if (((i % 4) == 0) && (i != 0))
         {
            printf("\n");
         }

         printf(" %02X", data[i]);
      }

      for(; i<4; ++i)
      {
         printf("   ");
      }

      printf(" : ");

      PLT::MIDI::In::messageIn(length, data);

      printf("\n");
   }

   virtual void noteOn(uint8_t channel, uint8_t note,  uint8_t velocity) override
   {
      printf(" CH%u NOTE ON  %3u %3u", channel, note, velocity);
   }

   virtual void notePressure(uint8_t channel, uint8_t note,  uint8_t value) override
   {
      printf(" CH%u NOTE AFT %3u %3u", channel, note, value);
   }

   virtual void noteOff(uint8_t channel, uint8_t note,  uint8_t velocity) override
   {
      printf(" CH%u NOTE OFF %3u %3u", channel, note, velocity);
   }

   virtual void controlChange(uint8_t channel, uint8_t index, uint8_t value) override
   {
      printf(" CH%u CTRL     %3u %3u", channel, index, value);
   }

   virtual void pitchBend(uint8_t channel, int16_t value) override
   {
      printf(" CH%u PITCHB   %5d", channel, value);
   }

   virtual void channelPressure(uint8_t channel, uint8_t value) override
   {
      printf(" CH%u CHAN AFT %3u", channel, value);
   }

   virtual void programChange(uint8_t channel, uint8_t index) override
   {
      printf(" CH%u PROGRAM  %3u", channel, index);
   }
};


int main()
{
   TestMidiIn midi_in;

   getchar();
}

