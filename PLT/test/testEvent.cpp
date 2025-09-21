//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
