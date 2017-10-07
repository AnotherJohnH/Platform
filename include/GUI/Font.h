//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#ifndef GUI_FONT_H
#define GUI_FONT_H

#include <cassert>
#include <cstdint>

#include "GUI/Vector.h"

namespace GUI {

//! Fixed width font
class Font
{
private:
public: // TODO make these private
   Vector          size;
   uint8_t         first;
   uint8_t         last;
   uint8_t         bpp;
   const uint8_t*  bitmaps;

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
   unsigned getBytesPerChar() const
   {
      return size.y * getBytesPerLine();
   }

public:
   //! Get default cell width (pixels)
   unsigned getWidth() const { return size.x; }

   //! Get default cell height (pixels)
   unsigned getHeight() const { return size.y; }

   //! Get bits per pixel in alpha-map data
   unsigned getBPP() const { return bpp; }

   unsigned getBitmapsSize() const
   {
      return getBytesPerChar() * (last - first + 1);
   }

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
      if ((ch < first) || (ch > last))
      {
         return 0;
      }

      return &bitmaps[getBytesPerChar() * (ch - first)];
   }

   uint8_t getPixel(uint8_t ch, unsigned x, unsigned y) const
   {
      const uint8_t* alpha = getAlphaMap(ch);
      const uint8_t* line  = alpha + getBytesPerLine() * y;
      unsigned pixels_per_byte = getPixelsPerByte();
      unsigned index  = x / pixels_per_byte;
      unsigned offset = pixels_per_byte - 1 - (x % pixels_per_byte);
      unsigned shift  = offset * bpp;
      uint8_t  mask   = (1<<bpp) - 1;
      return (line[index] >> shift) & mask;
   }

   void setPixel(uint8_t ch, unsigned x, unsigned y, uint8_t value)
   {
      uint8_t* alpha = (uint8_t*)getAlphaMap(ch);
      uint8_t* line  = alpha + getBytesPerLine() * y;
      unsigned pixels_per_byte = getPixelsPerByte();
      unsigned index  = x / pixels_per_byte;
      unsigned offset = pixels_per_byte - 1 - (x % pixels_per_byte);
      unsigned shift  = offset * bpp;
      uint8_t mask    = (1<<bpp) - 1;
      value &= mask;
      line[index] = line[index] & ~(mask<<shift);
      line[index] = line[index] |  (value<<shift);
   }
};

} // namespace GUI

#endif
