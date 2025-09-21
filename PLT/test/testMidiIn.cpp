//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "PLT/Event.h"
#include "PLT/Midi.h"


class TestMidiIn : public PLT::MIDI::In
{
private:
   virtual unsigned decode(const uint8_t* data, unsigned length) override
   {
      return STB::MIDI::disassemble(data, length);
   }
};


int main()
{
   TestMidiIn midi_in;

   return PLT::Event::mainLoop();
}
