//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cassert>
#include <cstdint>

#include "GUI/Vector.h"

namespace GUI {

//! Fixed width font
class Font
{
public:
   Font(Vector         size_,
        uint8_t        first_,
        uint8_t        last_,
        uint8_t        bpp_,
        const uint8_t* bitmaps_) 
      : size(size_)
      , first(first_)
      , last(last_)
      , bpp(bpp_)
      , bitmaps(bitmaps_)
   {
   }

   //! Get default cell width (pixels)
   unsigned getWidth() const { return size.x; }

   //! Get default cell height (pixels)
   unsigned getHeight() const { return size.y; }

   //! Get bits per pixel in alpha-map data
   unsigned getBPP() const { return bpp; }

   unsigned getBitmapsSize() const { return getBytesPerChar() * (last - first + 1); }

   //! Get width for a string (pixels)
   unsigned getWidth(const char* text) const
   {
      assert(text);

      unsigned w = 0;

      while(*text++)
      {
         w += getWidth();
      }

      return w;
   }

   //! Get raw alphamap data for a character
   //  Returns null if character is not defined in this font
   const uint8_t* getAlphaMap(uint8_t ch) const
   {
      if((ch < first) || (ch > last))
      {
         return 0;
      }

      return &bitmaps[getBytesPerChar() * (ch - first)];
   }

   uint8_t getPixel(uint8_t ch, unsigned x, unsigned y) const
   {
      const uint8_t* alpha           = getAlphaMap(ch);
      const uint8_t* line            = alpha + getBytesPerLine() * y;
      unsigned       pixels_per_byte = getPixelsPerByte();
      unsigned       index           = x / pixels_per_byte;
      unsigned       offset          = pixels_per_byte - 1 - (x % pixels_per_byte);
      unsigned       shift           = offset * bpp;
      uint8_t        mask            = (1 << bpp) - 1;
      return (line[index] >> shift) & mask;
   }

   void setPixel(uint8_t ch, unsigned x, unsigned y, uint8_t value)
   {
      uint8_t* alpha           = (uint8_t*)getAlphaMap(ch);
      uint8_t* line            = alpha + getBytesPerLine() * y;
      unsigned pixels_per_byte = getPixelsPerByte();
      unsigned index           = x / pixels_per_byte;
      unsigned offset          = pixels_per_byte - 1 - (x % pixels_per_byte);
      unsigned shift           = offset * bpp;
      uint8_t  mask            = (1 << bpp) - 1;
      value &= mask;
      line[index] = line[index] & ~(mask << shift);
      line[index] = line[index] | (value << shift);
   }

private:
   unsigned getPixelsPerByte() const
   {
      assert((bpp == 1) || (bpp == 2) || (bpp == 4) || (bpp == 8));
      return 8 / bpp;
   }

   //! Get length of alphamap data for each line in a char (bytes)
   unsigned getBytesPerLine() const
   {
      unsigned pixels_per_byte = getPixelsPerByte();
      return (size.x + pixels_per_byte - 1) / pixels_per_byte;
   }

   //! Get length of alphamap data for each char (bytes)
   unsigned getBytesPerChar() const { return size.y * getBytesPerLine(); }

   Vector         size;
   uint8_t        first;
   uint8_t        last;
   uint8_t        bpp;
   const uint8_t* bitmaps;
};

} // namespace GUI
