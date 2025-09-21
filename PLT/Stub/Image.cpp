//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Stub Image implementation

#include "PLT/Image.h"

namespace PLT {

unsigned Image::getPixelBits()
{
   return 4;
}

uint32_t Image::getPixel(unsigned x, unsigned y) const
{  
   return 0;
}

void Image::clear(STB::Colour rgb)
{
   defaultClear(rgb);
}

void Image::point(STB::Colour rgb, unsigned x, unsigned y)
{
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
   defaultLineBlit(pixel_mask, one, zero, x, y);
}

bool Image::save(const char* name) const
{
   return defaultSave(name);
}

} // namespace PLT
