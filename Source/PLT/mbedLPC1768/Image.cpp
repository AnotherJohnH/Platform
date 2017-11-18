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

#include "PLT/Image.h"

#include "MTL/chip/LPC1768/PALVideo.h"

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
   return byte & mask ? 0xFFFFFF : 0x000000;
}

void Image::setPixel(unsigned x, unsigned y, uint32_t rgb)
{
   unsigned index = (x/PIXELS_PER_BYTE) + y * pitch;
   uint8_t& byte = buffer[index ^ MTL::PAL_VIDEO_BYTE_SWAP];
   uint8_t  mask = 0x80>>(x % PIXELS_PER_BYTE);

   if (rgb != 0)
   {
      byte |= mask;
   }
   else
   {
      byte &= ~mask;
   }
}

} // namespace PLT
