//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstring>

#include "PLT/Image.h"

namespace PLT {

unsigned Image::getPixelBits() { return 4; }

uint32_t Image::getPixel(unsigned x, unsigned y) const
{
   uint8_t pair = buffer[(x/2) + y * pitch];
   uint8_t blackness = x & 1 ? (pair << 4) : (pair & 0xF0);
   uint8_t grey = 0xF0 - blackness;
   return (grey << 16) | (grey << 8) | grey;
}

void Image::point(STB::Colour rgb, unsigned x, unsigned y)
{
   uint8_t grey = 0xF - (STB::ColourDecode(rgb).grn() >> 4);
   uint8_t& pair = buffer[(x/2) + y * pitch];
   if(x & 1)
   {
      pair = (pair & 0xF0) | grey;
   }
   else
   {
      pair = (pair & 0x0F) | (grey << 4);
   }
}

void Image::clear(STB::Colour rgb)
{
   defaultClear(rgb);
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
   // TODO optimise this
   defaultBlit(source, x, y, w, h, src_x, src_y);
}

void Image::lineBlit(uint8_t pixel_mask, STB::Colour one, STB::Colour zero,
                     unsigned x, unsigned y)
{
   defaultLineBlit(pixel_mask, one, zero, x, y);
}

bool Image::save(const char* name) const
{
   return defaultSave(name);
}

} // namespace PLT
