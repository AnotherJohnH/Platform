//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Platform abstraction layer for access to MIDI devices

#pragma once

#include <cstdint>

#include "STB/MIDIInterface.h"


//! Platform abstraction layer
namespace PLT {

//! Access to MIDI devices
namespace MIDI {


class Interface : public ::MIDI::Interface
{
public:
   Interface();

   Interface(::MIDI::Instrument& instrument_, bool debug_ = false);

   ~Interface();

   bool connected() const;
   bool empty() const override;
   uint8_t rx() override;
   void tx(uint8_t byte) override;

private:
   struct Pimpl;
   Pimpl* pimpl;
};


} // namespace MIDI

} // namespace PLT
