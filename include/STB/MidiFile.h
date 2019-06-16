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

#ifndef STB_MIDI_FILE_H
#define STB_MIDI_FILE_H

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <string>

#include "STB/Endian.h"

namespace STB {

namespace MIDI {

//! A MIDI File
class File
{
public:
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

   //! MIDI file format
   //  0 => single multi-channel track
   //  1 => one or more simultanious tracks
   //  2 => one or more independant tracks
   unsigned getFormat() const { return data->format; }

   //! Get number of tracks
   unsigned getNumTracks() const { return data->ntrks; }

   //! Get time division
   uint16_t getDivision() const { return data->division; }

   const uint8_t* getTrackData(unsigned track_no, size_t& size)
   {
      const Chunk* chunk = &data->chunk;

      for(unsigned i=0; i<getNumTracks(); i++)
      {
         chunk = chunk->getNext();
         if (i == track_no)
         {
            size = chunk->size();
            return chunk->data();
         }
      }

      size = 0;
      return nullptr; 
   }

private:
   //! The header for a MIDI file chunk
   //  Should only be used to decode a MIDI file that is stored in a single contiguous
   //  block of memory
   class Chunk
   {
   public:
      Chunk(bool is_header)
      {
         type[0] = 'M';
         type[1] = 'T';
         type[2] = is_header ? 'h' : 'r';
         type[3] = is_header ? 'd' : 'k';
      }

      //! Get raw size of track (bytes)
      size_t size() const { return length; }

      //! Return a pointer to the data for the chunk
      const uint8_t* data() const { return (const uint8_t*)this + sizeof(Chunk); }

      //! Return a pointer to the byte after the last data byte in the chunk
      const uint8_t* end() const { return data() + size(); }

      //! Return a pointer to the next chunk
      const Chunk* getNext() const { return (const Chunk*)end(); }

   private:
      uint8_t    type[4];
      STB::Big32 length{0};
   };

   //! The header for a MIDI file
   struct Header
   {
      Chunk      chunk{/* is_header */true};
      STB::Big16 format{0};
      STB::Big16 ntrks{0};
      STB::Big16 division{0};
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
