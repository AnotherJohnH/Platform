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

#ifndef PLT_MIDI_H
#define PLT_MIDI_H

#include <cstdint>

namespace PLT {

namespace MIDI {


static const uint8_t  NOTE_OFF          = 0x80;
static const uint8_t  NOTE_ON           = 0x90;
static const uint8_t  POLY_KEY_PRESSURE = 0xA0;
static const uint8_t  CONTROL_CHANGE    = 0xB0;
static const uint8_t  PROGRAM_CHANGE    = 0xC0;
static const uint8_t  CHANNEL_PRESSURE  = 0xD0;
static const uint8_t  PITCH_BEND        = 0xE0;
static const uint8_t  SYSEX             = 0xF0;


//! MIDI input stream
class In
{
public:
   In(unsigned device_index = 0);
   ~In();

   bool isConnected() const { return connected; }

   virtual void noteOn(         uint8_t channel, uint8_t note,  uint8_t velocity) {}
   virtual void notePressure(   uint8_t channel, uint8_t note,  uint8_t value)    {}
   virtual void noteOff(        uint8_t channel, uint8_t note,  uint8_t velocity) {}
   virtual void controlChange(  uint8_t channel, uint8_t index, uint8_t value)    {}
   virtual void pitchBend(      uint8_t channel, int16_t value)                   {}
   virtual void channelPressure(uint8_t channel, uint8_t value)                   {}
   virtual void programChange(  uint8_t channel, uint8_t index)                   {}

   virtual void messageIn(unsigned length, uint8_t* data)
   {
      uint8_t channel = data[0] & 0x0F;
      uint8_t message = data[0] & 0xF0;

      switch(message)
      {
      case PLT::MIDI::NOTE_OFF:
         noteOff(channel, data[1], data[2]);
         break;

      case PLT::MIDI::POLY_KEY_PRESSURE:
         notePressure(channel, data[1], data[2]);
         break;

      case PLT::MIDI::NOTE_ON:
         if (data[2] != 0)
         {
            noteOn(channel, data[1], data[2]);
         }
         else
         {
            noteOff(channel, data[1], data[2]);
         }
         break;

      case PLT::MIDI::CHANNEL_PRESSURE:
         channelPressure(channel, data[1]);
         break;

      case PLT::MIDI::PITCH_BEND:
         pitchBend(channel, ((data[2] << 7) | data[1]) - 0x2000);
         break;

      case PLT::MIDI::CONTROL_CHANGE:
         controlChange(channel, data[1], data[2]);
         break;

      case PLT::MIDI::PROGRAM_CHANGE:
         programChange(channel, data[1]);
         break;

      default: break;
      }
   }

private:
   bool connected{false};
};


//! MIDI output stream
class Out
{
public:
   Out(unsigned device_index = 0);
   ~Out();

   bool isConnected() const { return connected; }

   void messageOut(unsigned length, const uint8_t* data);

private:
   bool connected{false};
};


} // namespace MIDI

} // namespace PLT

#endif
