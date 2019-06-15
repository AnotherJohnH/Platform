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

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <string>

#include "STB/Endian.h"

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
      uint8_t command{0};
   };

   // Channel voice messages
   virtual void noteOn(         uint8_t channel, uint8_t note, uint8_t velocity) {}
   virtual void notePressure(   uint8_t channel, uint8_t note, uint8_t value) {}
   virtual void noteOff(        uint8_t channel, uint8_t note, uint8_t velocity) {}
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

   virtual unsigned decode(const uint8_t* data, unsigned length);

   static unsigned decodeVarLength(const uint8_t* first, uint32_t& value)
   {
      value = 0;

      const uint8_t* ptr = first;

      while(true)
      {
         uint8_t byte = *ptr++;
         value        = (value << 7) | (byte & 0x7F);
         if((byte & 0x80) == 0) return ptr - first;
      }
   }

   void resetState() { state.command = 0; }

   const State& getState() { return state; }

   void setState(const State& state_) { state = state_; }

protected:
   unsigned decodeCommand(const uint8_t* data, unsigned length);

   State state;
};


//! A MIDI File
class File
{
public:
   //! The header for a MIDI file chunk
   //  Should only be used to decode a MIDI file that is stored in a single contiguous
   //  block of memory
   class Chunk
   {
   public:
      Chunk(bool is_header = true)
      {
         type[0] = 'M';
         type[1] = 'T';
         type[2] = is_header ? 'h' : 'r';
         type[3] = is_header ? 'd' : 'k';
      }

      //! Looks like a valid MIDI chunk
      bool isValid() const { return (type[0] == 'M') && (type[1] == 'T'); }

      //! Is a MIDI header chunk
      bool isHeader() const { return isValid() && (type[2] == 'h') && (type[3] == 'd'); }

      //! Is a MIDI track chunk
      bool isTrack() const { return isValid() && (type[2] == 'r') && (type[3] == 'k'); }

      //! Return a pointer to the data for the chunk
      const uint8_t* data() const { return (const uint8_t*)this + sizeof(Chunk); }

      //! Return a pointer to the byte after the last data byte in the chunk
      const uint8_t* end() const { return data() + length; }

      //! Return a pointer to the next chunk
      const Chunk* getNext() const { return (const Chunk*)end(); }

   private:
      uint8_t    type[4];
      STB::Big32 length{0};
   };

   File() = default;

   ~File()
   {
      delete[] data;
   }

   bool load(const std::string& filename)
   {
      // Open file
      FILE* fp = fopen(filename.c_str(), "r");
      if (fp == nullptr) return error("Failed to open file");

      // Get size (bytes)
      if (fseek(fp, 0, SEEK_END) != 0) return error("Failed to fseek");
      long size = ftell(fp);
      if (size <= 0)                   return error("Failed to ftell");
      if (fseek(fp, 0, SEEK_SET) != 0) return error("Failed to fseek");

      if (data != nullptr)
      {
         delete[] data;
      }

      data = (Header*) new uint8_t[size];

      // Read file
      if (fread((void*)data, size, 1, fp) != 1) return error("Failed to fread");

      fclose(fp);
      fp = nullptr;

      return true;
   }

   const Chunk* getFirstChunk() const { return &data->chunk; }

   uint16_t     getFormat() const     { return data->format; }

   uint16_t     getNumTracks() const  { return data->ntrks; }

   uint16_t     getDivision() const   { return data->division; }

private:
   //! The header for a MIDI file
   struct Header
   {
      Chunk      chunk;
      STB::Big16 format;
      STB::Big16 ntrks;
      STB::Big16 division;
   };

   FILE*   fp{nullptr};
   Header* data{nullptr};

   bool error(const std::string& message)
   {
      fprintf(stderr, "ERR: MIDI %s\n", message.c_str());
      if (fp != nullptr)
      {
         fclose(fp);
         fp = nullptr;
      }
      return false;
   }
};


//! Disassemble MIDI stream
extern unsigned disassemble(const uint8_t* data, unsigned length);


} // namespace MIDI

} // namespace STB

#endif
