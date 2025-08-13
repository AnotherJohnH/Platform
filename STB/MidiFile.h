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

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <string>
#include <vector>

#include "STB/Endian.h"
#include "STB/MidiDecoder.h"

namespace STB {

namespace MIDI {

struct TrackPtr
{
   TrackPtr() = default;

   bool isPlaying() const { return ptr < end; }

   size_t getSizeLeft() const { return end - ptr; }

   void clear()
   {
      ptr = end = nullptr;
      t = 0;
   }

   void init(const uint8_t* start, size_t bytes)
   {
      ptr = start;
      end = start + bytes;
      t   = 0;
   }

   const uint8_t* ptr{};
   const uint8_t* end{};
   unsigned       t{0};
};

//! A MIDI File
class File
{
public:
   File() = default;

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

      image.resize(size);

      // Read file
      if (fread(image.data(), size, 1, fp) != 1) return error("Failed to fread");

      fclose(fp);
      fp = nullptr;

      header = (Header*)image.data();

      return true;
   }

   //! MIDI file format
   //  0 => single multi-channel track
   //  1 => one or more simultanious tracks
   //  2 => one or more independant tracks
   unsigned getFormat() const { return header->format; }

   //! Get number of tracks
   unsigned getNumTracks() const { return header->ntrks; }

   //! Get time division
   uint16_t getDivision() const { return header->division; }

   //! Get raw data for a track
   bool getTrackData(unsigned track_no, TrackPtr* tp) const
   {
      const Chunk* chunk = &header->chunk;

      for(unsigned i = 0; i < getNumTracks(); i++)
      {
         chunk = chunk->getNext();

         if (i == track_no)
         {
            tp->init(chunk->data(), chunk->size());
            return true;
         }
      }

      tp->clear();
      return false;
   }

   void decodeTrack(unsigned track_no, Decoder* decoder) const
   {
      TrackPtr tp;

      getTrackData(track_no, &tp);

      decoder->resetState();

      while(tp.isPlaying())
      {
         tp.ptr += decoder->decodeDeltaT(tp.ptr);
         tp.ptr += decoder->decode(tp.ptr, tp.getSizeLeft());
      }
   }

   bool playTrack(unsigned track_no, Decoder* decoder, TrackPtr* tp)
   {
      if (tp->isPlaying())
      {
         tp->t++;
      }
      else
      {
         getTrackData(track_no, tp);

         decoder->resetState();
      }

      while(tp->t >= decoder->getTime())
      {
         if (not tp->isPlaying())
         {
            return false;;
         }

         tp->ptr += decoder->decodeDeltaT(tp->ptr);
         tp->ptr += decoder->decode(tp->ptr, tp->getSizeLeft());
      }

      return true;
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

   bool error(const char* message)
   {
      fprintf(stderr, "ERR: MIDI %s\n", message);
      if (fp != nullptr)
      {
         fclose(fp);
         fp = nullptr;
      }
      return false;
   }

   FILE*                fp{nullptr};
   std::vector<uint8_t> image{};
   Header*              header{nullptr};
};

} // namespace MIDI

} // namespace STB
