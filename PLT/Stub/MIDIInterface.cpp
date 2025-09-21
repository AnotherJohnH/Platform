//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Stub MIDI implementation

#include "PLT/MIDIInterface.h"

namespace PLT {

namespace MIDI {

Interface::Interface(::MIDI::Instrument& instrument_)
   : ::MIDI::Interface(instrument_)
{
}

Interface::~Interface() {}

bool Interface::connected() const { return false; }

bool Interface::empty() const { return true; }

uint8_t Interface::rx() { return 0; }

void Interface::tx(uint8_t byte) {}


} // namespace MIDI

} // namespace PLT
