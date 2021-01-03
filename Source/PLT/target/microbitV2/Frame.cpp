//------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
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

// Frame implementation for mictobitV2

#include "PLT/Frame.h"

#include "MTL/Button.h"
#include "MTL/nrF52/Timer.h"
#include "MTL/Leds.h"

static MTL::Button<MTL::BTN_A> btn_a;
static MTL::Button<MTL::BTN_B> btn_b;
static MTL::Leds               leds;

LEDS_ATTACH_IRQ(leds)

namespace PLT {

class Frame::Impl
{
public:
   Impl() = default;

   void setGenerator(Generator* generator_)
   {
      generator = generator_;

      generator->getConfig(width, height, interlace);
   }

   void refresh()
   {
      if (generator == nullptr) return;

      if (btn_a)
      {
         if (win_x != 0) win_x -= 1;
      }
      else if (btn_b)
      {
         win_x += 1;
      }

#if 0
      else if (dbgctl == 3)
      {
         win_y += 2;
      }
      else if (dbgctl == 4)
      {
         if (win_y != 0) win_y -= 2;
      }
#endif

      unsigned win_x_word   = win_x / 32;
      unsigned win_x_offset = win_x % 32;

      uint8_t  frame[5] = {0};

      unsigned words_per_line = width / 32;

      generator->startField(field);

      for(unsigned line = field; line < height; line += 2)
      {
         generator->startLine(line);

         for(unsigned word = 0; word < words_per_line; ++word)
         {
            uint32_t pixels = generator->getPixelData_1BPP();

            if ((line >= win_y) && (line < (win_y + 10)))
            {
               unsigned y = (line - win_y) / 2;

               if (word == win_x_word)
               {
                  if (win_x_offset <= 27)
                  {
                     frame[y] = (pixels >> (27 - win_x_offset)) & 0x1F;
                  }
                  else
                  {
                     frame[y] = (pixels << (win_x_offset - 27)) & 0x1F;
                  }
               }
               else if ((win_x_offset > 27) && (word == (win_x_word + 1)))
               {
                  frame[y] |= pixels >> (32 + 27 - win_x_offset);
               }
            }
         }
      }

      field ^= 1;

      leds.write(frame[0], frame[1], frame[2], frame[3], frame[4]);
   }

private:
   PLT::Frame::Generator* generator{};
   uint16_t               width, height;
   bool                   interlace;
   unsigned               field{0};
   unsigned               win_x{0};
   unsigned               win_y{20};
};

//----------------------------------------------------------------------

Frame::Frame(const char* title_, unsigned width_, unsigned height_, uint32_t flags_)
   : Image(width_, height_)
{
   static Frame::Impl impl;

   pimpl = &impl;
}

Frame::~Frame()
{
}

void* Frame::getHandle() const
{
   return nullptr;
}

uint32_t Frame::getId() const
{
   return 0;
}

void Frame::setTitle(const char* title_)
{
}

void Frame::setFlags(uint32_t flags_)
{  
}  

void Frame::setVisible(bool visible_)
{
}

void Frame::resize(unsigned width_, unsigned height_)
{
}

void Frame::refresh()
{
   pimpl->refresh();
}

void Frame::setGenerator(Generator* generator_)
{
   pimpl->setGenerator(generator_);
}

} // namespace PLT
