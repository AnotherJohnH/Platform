//------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
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
#include <cstdlib>
#include <cstring>

#include "STB/Midi.h"

class MidiFile : public STB::MIDI::File
{
public:
   void dump()
   {
      printf("format=%04x ntrks=%04x division=%04x\n",
             getFormat(), getNumTracks(), getDivision());

      for(unsigned i=0; i<getNumTracks(); ++i)
      {
         dumpTrack(i);
      }
   }

private:
   void dumpTrack(unsigned i)
   {
      printf("TRACK %u\n", i);

      STB::MIDI::TrackPtr tp;

      getTrackData(i, &tp);

      const uint8_t* raw = tp.ptr;

      while(tp.isPlaying())
      {
         uint32_t delta_t;

         const uint8_t* start = tp.ptr;
         const uint8_t* event = start + STB::MIDI::Decoder::decodeVarLength(start, delta_t);

         printf("%06X : dt=%05u ", unsigned(start - raw), delta_t);
         tp.ptr = event + STB::MIDI::disassemble(event, tp.end - event);
      }
   }
};


int main(int argc, const char* argv[])
{
   if (argc != 2)
   {
      fprintf(stderr, "ERROR: %s <filename>\n", argv[0]);
      exit(1);
   }

   MidiFile midi_file;

   if (midi_file.load(argv[1]))
   {
      midi_file.dump();
   }

   return 0;
}
