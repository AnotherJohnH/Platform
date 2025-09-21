//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cassert>
#include <cstdio>

#include "PLT/Frame.h"
#include "PLT/Event.h"

int main(int argc, const char* argv[])
{
   PLT::Frame frame("PLT::Frame test", 400, 300);

   frame.clear(STB::BLACK);

   for(unsigned y = 0; y < frame.getHeight(); y++)
   {
      for(unsigned x = 0; x < frame.getWidth(); x++)
      {
         frame.point((x ^ y) & 8 ? STB::WHITE : STB::BLACK, x, y);
      }
   }

   frame.span(STB::GREEN, 0, 149, 400);
   frame.span(STB::GREEN, 0, 150, 400);
   frame.span(STB::GREEN, 0, 151, 400);

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
