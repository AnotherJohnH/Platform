//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cassert>
#include <cstdio>

#include "PLT/Frame.h"
#include "PLT/Event.h"

#include "GUI/Font/New.h"


template <unsigned WIDTH, unsigned HEIGHT>
class Screen : public PLT::Frame::Generator
{
public:
   Screen()
   {
      uint8_t ch = ' ';

      for(unsigned y=0; y<ROWS; y++)
      {
         for(unsigned x=0; x<COLS; x++)
         {
            data[x + y*COLS] = ch++;
            if (ch == 0x80) ch = ' ';
         }
      }

      frame.setGenerator(this);
   }

   void refresh()
   {
      frame.refresh();
   }

private:
   static const unsigned COLS = WIDTH  / 8;
   static const unsigned ROWS = HEIGHT / 8;

   PLT::Frame  frame{"PLT::Frame scanner test", WIDTH, HEIGHT};
   uint8_t     data[COLS * ROWS];

   virtual void getRawPixels(uint8_t* buffer, unsigned y) override
   {
      unsigned row  = y / 8;
      unsigned line = y % 8;

      for(unsigned col=0; col<COLS; col++)
      {
         uint8_t ch = data[col + row * COLS];
         const uint8_t* alpha_map = GUI::font_new8.getAlphaMap(ch);
         uint8_t pixels = alpha_map[line];

         switch(frame.getPixelBits())
         {
         case 1:
            buffer[col ^ 1] = pixels;
            break;

         case 4:
            {
               uint32_t* buffer32 = (uint32_t*)buffer;
               uint32_t  raw = 0;
               for(unsigned x=0; x<8; x++)
               {
                  raw = (raw << 4) | (pixels & 0x80 ? 0xF : 0x0);
                  pixels <<= 1;
               }
               buffer32[col] = raw;
            }
            break;

         case 32:
            {
               uint32_t* buffer32 = (uint32_t*)buffer;
               for(unsigned x=0; x<8; x++)
               {
                  buffer32[col*8 + x] = pixels & 0x80 ? STB::WHITE : STB::BLACK;
                  pixels <<= 1;
               }
            }
            break;
         }
      }
   }
};


int main(int argc, const char* argv[])
{
   Screen<640,256> screen;

   screen.refresh();

   return PLT::Event::eventLoop();
}
