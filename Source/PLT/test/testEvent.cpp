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

#include <cstdio>

#include "PLT/Event.h"
#include "PLT/Frame.h"


void callback(const PLT::Event::Message& event, void* ptr)
{
   static unsigned ticks = 0;

   PLT::Frame* frame = static_cast<PLT::Frame*>(ptr);
   STB::Colour rgb;

   switch(event.type)
   {
   case PLT::Event::KEY_DOWN:
      printf("KEY_DOWN     %02X\n", event.code);
      for(unsigned y=248; y<256; y++)
         frame->point(STB::YELLOW, event.code, y);
      break;

   case PLT::Event::KEY_UP:
      printf("KEY_UP       %02X\n", event.code);
      for(unsigned y=248; y<256; y++)
         frame->point(STB::BLACK, event.code, y);
      break;

   case PLT::Event::BUTTON_DOWN:
      printf("BUTTON_DOWN  %d %u %u\n", event.code, event.x, event.y);
      break;

   case PLT::Event::BUTTON_UP:
      printf("BUTTON_UP    %d %u %u\n", event.code, event.x, event.y);
      break;

   case PLT::Event::POINTER_MOVE:
      printf("POINTER_MOVE %u %u\n",    event.x, event.y);
      frame->point(STB::GREEN, event.x, event.y);
      break;

   case PLT::Event::TIMER:
      printf("TIMER\n");
      ticks = (ticks + 1) % frame->getWidth();
      rgb = frame->getPixel(ticks, 0);
      frame->point(rgb ? STB::BLACK : STB::WHITE, ticks, 0);
      frame->point(rgb ? STB::BLACK : STB::WHITE, ticks, 1);
      break;

   case PLT::Event::RESIZE:
      printf("RESIZE\n");
      break;

   case PLT::Event::QUIT:
      printf("QUIT\n");
      break;

   default:
      break;
   }

   frame->refresh();
}


int main(int argc, const char* argv[])
{
   PLT::Frame frame("PLT::Event Test", 256, 256,
                    PLT::Frame::SCALE_X(2) | PLT::Frame::SCALE_Y(2));

   frame.clear(STB::BLACK);

   PLT::Event::setTimer(500);

   return PLT::Event::eventLoop(callback, &frame);
}
