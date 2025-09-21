//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
