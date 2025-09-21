//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file Midi.h
//! \brief Platform abstraction layer for access to MIDI devices

#pragma once

#include <cstdint>

#include "STB/MidiDecoder.h"

//! Platform abstraction layer
namespace PLT {

//! Access to MIDI devices
namespace MIDI {


//! Base class for MIDI I/O streams
class IOBase
{
public:
   //! Check for successfully opened connection
   bool isConnected() const { return connected; }

protected:
   bool connected{false};
};


//! MIDI input stream
class In : public IOBase
         , public STB::MIDI::Decoder
{
public:
   //! Open MIDI input stream
   //
   // \param device_index Platform specific device id
   In(unsigned device_index = 0);

   //! Close MIDI input stream
   ~In();
};


//! MIDI output stream
class Out : public IOBase
{
public:
   //! Open MIDI output stream
   //
   // \param device_index Platform specific device id
   Out(unsigned device_index = 0);

   //! Close MIDI output stream
   ~Out();

   //! Send a MIDI message
   void messageOut(unsigned length, const uint8_t* data);
};


} // namespace MIDI

} // namespace PLT

