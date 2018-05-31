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

// \brief mbed LPC1768 Image implementation

#include <cstring>

#include "PLT/Image.h"

#include "../../MTL/chip/LPC1768/PALVideo.h"

namespace PLT {

static const unsigned BITS_PER_PIXEL = 1;
static const unsigned PIXELS_PER_BYTE = 8/BITS_PER_PIXEL;

unsigned Image::getPixelBits()
{
   return BITS_PER_PIXEL;
}

uint32_t Image::getPixel(unsigned x, unsigned y) const
{  
   unsigned index = (x/PIXELS_PER_BYTE) + y * pitch;
   uint8_t byte = buffer[index ^ MTL::PAL_VIDEO_BYTE_SWAP];
   uint8_t mask = 0x80>>(x % PIXELS_PER_BYTE);
   return byte & mask ? STB::WHITE : STB::WHITE;
}

void Image::clear(STB::Colour rgb)
{
   uint8_t grn = STB::ColourDecode(rgb).grn();

   for(unsigned y=0; y<height; y++)
   {
      uint8_t byte;

           if (grn >= 0xE0) { byte = 0xFF; }
      else if (grn >= 0xA0) { byte = y & 1 ? 0xFF : 0x55; }
      else if (grn >= 0x60) { byte = y & 1 ? 0xAA : 0x55; }
      else if (grn >= 0x20) { byte = y & 1 ? 0x00 : 0x55; }
      else                  { byte = 0x00; }

      unsigned x=0;

      for(; x<((width + 7) & 7); x += 8)
      {
         unsigned index = (x/PIXELS_PER_BYTE) + y * pitch;
         buffer[index ^ MTL::PAL_VIDEO_BYTE_SWAP] = byte;
      }

      for(; x<width; x++)
      {
         point(rgb, x, y);
      }
   }
}

void Image::point(uint32_t rgb, unsigned x, unsigned y)
{
   unsigned index = (x/PIXELS_PER_BYTE) + y * pitch;
   uint8_t& byte = buffer[index ^ MTL::PAL_VIDEO_BYTE_SWAP];
   uint8_t  mask = 0x80>>(x % PIXELS_PER_BYTE);
   uint8_t  grn  = STB::ColourDecode(rgb).grn();
   bool     set;

        if (grn >= 0xE0) { set = true; }
   else if (grn >= 0xA0) { set = (x | y) & 1; }
   else if (grn >= 0x60) { set = (x ^ y) & 1; }
   else if (grn >= 0x20) { set = (x & y) & 1; }
   else                  { set = false; }

   if (set)
   {
      byte |= mask;
   }
   else
   {
      byte &= ~mask;
   }
}

void Image::span(STB::Colour rgb, unsigned x1, unsigned y, unsigned x2)
{
   defaultSpan(rgb, x1, y, x2);
}

void Image::blit(const Image& source,
                 unsigned x, unsigned y,
                 unsigned w, unsigned h,
                 unsigned src_x, unsigned src_y)
{
   defaultBlit(source, x, y, w, h, src_x, src_y);
}

void Image::lineBlit(uint8_t pixel_mask, STB::Colour one, STB::Colour zero,
                     unsigned x, unsigned y)
{
   // TODO optimise as a single byte write when aligned
   defaultLineBlit(pixel_mask, one, zero, x, y);
}

bool Image::save(const char* name)
{
   return defaultSave(name);
}

} // namespace PLT
