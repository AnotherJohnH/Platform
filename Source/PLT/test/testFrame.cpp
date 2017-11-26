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

#include <cassert>
#include <cstdio>

#include "PLT/Frame.h"
#include "PLT/Event.h"

int main(int argc, const char* argv[])
{
   PLT::Frame frame("PLT::Frame test", 400, 300);

   for(unsigned y = 0; y < frame.getHeight(); y++)
   {
      for(unsigned x = 0; x < frame.getWidth(); x++)
      {
         frame.point((x ^ y) & 8 ? 0xFFFFFF : 0x000000, x, y);
      }
   }

   frame.span(0xFFFFFF, 0, 149, 400);
   frame.span(0xFFFFFF, 0, 150, 400);
   frame.span(0xFFFFFF, 0, 151, 400);

   frame.refresh();

   unsigned pitch;

   printf("Bits       : %u bits/pixel\n", PLT::Frame::getPixelBits());
   printf("Width      : %u pixels\n", frame.getWidth());
   printf("Height     : %u pixels\n", frame.getHeight());
   printf("Handle     : %p\n", frame.getHandle());
   printf("Storage    : %p\n", frame.getStorage(pitch));
   printf("Pitch      : %u bytes\n", pitch);
   printf("Pixel(0,0) : 0x%08X\n", unsigned(frame.getPixel(0, 0)));
   printf("Pixel(0,8) : 0x%08X\n", unsigned(frame.getPixel(0, 8)));

   assert((frame.getPixel(0, 0) & 0xFFFFFF) == 0x000000);
   assert((frame.getPixel(0, 8) & 0xFFFFFF) == 0xFFFFFF);

   return PLT::Event::eventLoop();
}
