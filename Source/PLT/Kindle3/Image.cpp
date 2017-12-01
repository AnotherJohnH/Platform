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

#include <cstring>

#include "PLT/Image.h"

namespace PLT {

unsigned Image::getPixelBits() { return 4; }

uint32_t Image::getPixel(unsigned x, unsigned y) const
{
   uint8_t pair = buffer[(x + y * pitch) / 2];
   uint8_t grey = x & 1 ? (pair << 4) : (pair & 0xF0);
   return (grey << 16) | (grey << 8) | grey;
}

void Image::clear(STB::Colour rgb)
{
   uint8_t grey = 0xF - (STB::ColourDecode(rgb).grn() >> 4);
   memset(buffer, (grey<<4) | grey, pitch * height);
}

void Image::point(STB::Colour rgb, unsigned x, unsigned y)
{
   uint8_t& pair = buffer[(x + y * pitch) / 2];
   uint8_t grey = 0xF - (STB::ColourDecode(rgb).grn() >> 4);
   if(x & 1)
   {
      pair = (pair & 0xF0) | grey;
   }
   else
   {
      pair = (pair & 0x0F) | (grey << 4);
   }
}

void Image::span(STB::Colour rgb, unsigned x1, unsigned y, unsigned x2)
{
   defaultSpan(rgb, x1, y, x2);
}

void Image::blit(unsigned x, unsigned y, unsigned src_offset, unsigned src_width,
                 const Image& source)
{
   defaultBlit(x, y, src_offset, src_width, source);
}

} // namespace PLT
