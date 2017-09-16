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

#ifndef STB_MIDI_H
#define STB_MIDI_H

#include <cstdint>

#include "STB/Endian.h"

namespace STB {

namespace MIDI {


static const uint8_t  NOTE_OFF          = 0x80;
static const uint8_t  NOTE_ON           = 0x90;
static const uint8_t  POLY_KEY_PRESSURE = 0xA0;
static const uint8_t  CONTROL_CHANGE    = 0xB0;
static const uint8_t  PROGRAM_CHANGE    = 0xC0;
static const uint8_t  CHANNEL_PRESSURE  = 0xD0;
static const uint8_t  PITCH_BEND        = 0xE0;
static const uint8_t  SYSEX             = 0xF0;

enum SystemEvent
{
   TUNE_REQUEST   = 6,
   CLOCK          = 8,
   START          = 10,
   CONTINUE       = 11,
   STOP           = 12,
   ACTIVE_SENSING = 14,
   RESET          = 15
};


inline unsigned decodeVarLength(const uint8_t* first, uint32_t& value)
{
   value = 0;

   const uint8_t* ptr = first;

   while(true)
   {
      uint8_t byte = *ptr++;
      value = (value << 7) | (byte & 0x7F);
      if ((byte & 0x80) == 0) return ptr - first;
   }
}


//! The header for a MIDI file chunk
//  Should only be used to decode a MIDI file that is stored in a single contiguous
//  block of memory
struct FileChunk
{
    FileChunk(bool is_header = true)
    {
        type[0] = 'M';
        type[1] = 'T';
        type[2] = is_header ? 'h' : 'r';
        type[3] = is_header ? 'd' : 'k';
    }

    //! Looks like a valid MIDI chunk
    bool isValid()  const { return (type[0] == 'M') && (type[1] == 'T'); }

    //! Is a MIDI header chunk
    bool isHeader() const { return isValid() && (type[2] == 'h') && (type[3] == 'd'); }

    //! Is a MIDI track chunk
    bool isTrack()  const { return isValid() && (type[2] == 'r') && (type[3] == 'k'); }

    //! Return a pointer to the data for the chunk
    const uint8_t* data()      const { return (const uint8_t*)this + sizeof(FileChunk); }
   
    //! Return a pointer to the byte after the last data byte in the chunk
    const uint8_t* end()       const { return data() + length; }
   
    //! Return a pointer to the next chunk
    const FileChunk* getNext() const { return (const FileChunk*)end(); }

    uint8_t      type[4];
    STB::Big32   length{0};
};


//! The header for a MIDI file
struct FileHeader
{
   FileChunk   chunk;
   STB::Big16  format;
   STB::Big16  ntrks;
   STB::Big16  division;

   //! Allocate memory for a complete MIDI file
   static FileHeader* construct(size_t size)
   {
      return (FileHeader*)(new uint8_t[size]);
   }
};


//! MIDI stream handler
class Handler
{
public:
   // Channel voice messages
   virtual void noteOn(         uint8_t channel, uint8_t note,  uint8_t velocity)      {}
   virtual void notePressure(   uint8_t channel, uint8_t note,  uint8_t value)         {}
   virtual void noteOff(        uint8_t channel, uint8_t note,  uint8_t velocity)      {}
   virtual void controlChange(  uint8_t channel, uint8_t index, uint8_t value)         {}
   virtual void pitchBend(      uint8_t channel, int16_t value)                        {}
   virtual void channelPressure(uint8_t channel, uint8_t value)                        {}
   virtual void programChange(  uint8_t channel, uint8_t index)                        {}

   // Channel mode messages
   virtual void localControl(uint8_t channel, bool on)              {}
   virtual void allNotesOff( uint8_t channel)                       {}
   virtual void omniMode(    uint8_t channel, bool on)              {}
   virtual void monoMode(    uint8_t channel, uint8_t num_channels) {}
   virtual void polyMode(    uint8_t channel)                       {}

   // System messages
   virtual void sysExcl(uint32_t length, const uint8_t* data) {}
   virtual void sysEvent(SystemEvent event)                   {}
   virtual void songPosition(uint16_t pos)                    {}
   virtual void songSelect(uint8_t song)                      {}

   virtual unsigned messageIn(const uint8_t* data, unsigned length);

private:
   unsigned decode(uint8_t command_, const uint8_t* data, unsigned length);

   uint8_t command{0};
};


} // namespace MIDI

} // namespace STB

#endif
