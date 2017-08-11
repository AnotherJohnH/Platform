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

#include "PLT/FrameBase.h"

namespace PLT {

unsigned FrameBase::getPixelBits()
{
   return 4;
}

uint32_t FrameBase::getPixel(unsigned x, unsigned y) const
{  
   uint8_t pair = buffer[(x + y * pitch)/2];
   uint8_t grey = x & 1 ? (pair << 4) : (pair & 0xF0);
   return (grey<<16) | (grey<<8) | grey;
}

void FrameBase::setPixel(unsigned x, unsigned y, uint32_t rgb)
{
   uint8_t& pair = buffer[(x + y * pitch)/2];
   uint8_t grey = 0xf - ((rgb >> 12) & 0xF);
   if (x & 1)
   {
      pair = (pair & 0xF0) | grey;
   }
   else
   {
      pair = (pair & 0x0F) | (grey<<4);
   }
}

} // namespace PLT

