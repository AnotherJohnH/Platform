//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace STB {

namespace MIDI {


// MIDI event types
static const uint8_t NOTE_OFF          = 0x80;
static const uint8_t NOTE_ON           = 0x90;
static const uint8_t POLY_KEY_PRESSURE = 0xA0;
static const uint8_t CONTROL_CHANGE    = 0xB0;
static const uint8_t PROGRAM_CHANGE    = 0xC0;
static const uint8_t CHANNEL_PRESSURE  = 0xD0;
static const uint8_t PITCH_BEND        = 0xE0;
static const uint8_t SYSTEM            = 0xF0;


// System event sub-types
enum class SystemEvent
{
   TUNE_REQUEST   = 0x6,
   CLOCK          = 0x8,
   START          = 0x9,
   CONTINUE       = 0xB,
   STOP           = 0xC,
   ACTIVE_SENSING = 0xE,
   RESET          = 0xF
};


// System meta event sub-types (with text)
enum class TextEvent
{
   TEXT       = 0x01,
   COPYRIGHT  = 0x02,
   TRACK_NAME = 0x03,
   INSTRUMENT = 0x04,
   LYRIC      = 0x05,
   MARKER     = 0x06,
   CUE        = 0x07
};


//! MIDI byte stream decoder
class Decoder
{
public:
   struct State
   {
      uint32_t t{0};
      uint8_t  command{0};
   };

   // Channel voice messages
   virtual void noteOn(         uint8_t channel, uint8_t note,  uint8_t velocity) {}
   virtual void notePressure(   uint8_t channel, uint8_t note,  uint8_t value) {}
   virtual void noteOff(        uint8_t channel, uint8_t note,  uint8_t velocity) {}
   virtual void controlChange(  uint8_t channel, uint8_t index, uint8_t value) {}
   virtual void pitchBend(      uint8_t channel, int16_t value) {}
   virtual void channelPressure(uint8_t channel, uint8_t value) {}
   virtual void programChange(  uint8_t channel, uint8_t index) {}

   // Channel mode messages
   virtual void localControl(uint8_t channel, bool on) {}
   virtual void allNotesOff( uint8_t channel) {}
   virtual void omniMode(    uint8_t channel, bool on) {}
   virtual void monoMode(    uint8_t channel, uint8_t num_channels) {}
   virtual void polyMode(    uint8_t channel) {}

   // System messages
   virtual void sysExcl(uint32_t length, const uint8_t* data) {}
   virtual void sysEvent(SystemEvent event) {}
   virtual void songPosition(uint16_t pos) {}
   virtual void songSelect(uint8_t song) {}

   // File meta messages
   virtual void textEvent(TextEvent event, const char*, unsigned length) {}
   virtual void todo() {}

   //! Get time of current event
   uint32_t getTime() const { return state.t; }

   unsigned decodeDeltaT(const uint8_t* ptr)
   {
      uint32_t delta_t;
      unsigned n = STB::MIDI::Decoder::decodeVarLength(ptr, delta_t);
      state.t += delta_t;
      return n;
   }

   virtual unsigned decode(const uint8_t* data, unsigned length);

   static unsigned decodeVarLength(const uint8_t* ptr, uint32_t& value)
   {
      value = 0;

      for(unsigned n = 0; true;)
      {
         uint8_t byte = ptr[n++];

         value = (value << 7) | (byte & 0b01111111);

         if ((byte & 0b10000000) == 0)
            return n;
      }
   }

   const State& getState() const { return state; }

   void resetState()
   {
      state.t       = 0;
      state.command = 0;
   }

   void setState(const State& state_) { state = state_; }

protected:
   unsigned decodeCommand(const uint8_t* data, unsigned length);

   State state;
};

//! Disassemble MIDI stream
extern unsigned disassemble(const uint8_t* data, unsigned length);

} // namespace MIDI

} // namespace STB
