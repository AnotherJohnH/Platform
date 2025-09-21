//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "GUI/Frame.h"
#include "GUI/Colour.h"
#include "GUI/Font/Teletext.h"

#include "PLT/Event.h"

int main(int argc, const char* argv[])
{
   GUI::Frame frame("GUI::Frame Test", 400, 300);

   frame.clear(STB::BLACK);

   frame.fillRect(STB::RED,         10,  10,  42,  42);
   frame.fillRect(STB::GREEN,       42,  10,  74,  42);
   frame.fillRect(STB::YELLOW,      74,  10, 106,  42);
   frame.fillRect(STB::BLUE,       106,  10, 138,  42);
   frame.fillRect(STB::MAGENTA,    138,  10, 170,  42);
   frame.fillRect(STB::CYAN,       170,  10, 202,  42);
   frame.fillRect(STB::WHITE,      202,  10, 234,  42);

   frame.fillRect(GUI::BACKGROUND,  10,  50,  42,  82);
   frame.fillRect(GUI::FOREGROUND,  42,  50,  74,  82);
   frame.fillRect(GUI::DARK,        74,  50, 106,  82);
   frame.fillRect(GUI::SHADOW,     106,  50, 138,  82);
   frame.fillRect(GUI::FACE,       138,  50, 170,  82);
   frame.fillRect(GUI::LIGHT,      170,  50, 202,  82);
   frame.fillRect(GUI::HILIGHT,    202,  50, 234,  82);

   for(unsigned x = 0; x < 256; x++)
   {
      frame.drawLine(STB::GREY(x), 10 + x, 90, 10 + x, 122);
   }

   for(unsigned x = 0; x < 256; x += 4)
   {
      frame.drawPoint(STB::WHITE, 10 + x, 130);
   }

   frame.fillRect(STB::WHITE, 10, 140, 70, 200);
   frame.drawRect(STB::BLACK, 11, 141, 68, 198);
   frame.drawRect(STB::BLACK, 14, 144, 65, 195);

   for(unsigned i = 0; i <= 60; i += 10)
   {
      frame.drawLine(STB::WHITE, 140, 170, 110 + i, 140);
      frame.drawLine(STB::WHITE, 140, 170, 170, 140 + i);
      frame.drawLine(STB::WHITE, 140, 170, 110 + i, 200);
      frame.drawLine(STB::WHITE, 140, 170, 110, 140 + i);
   }

   frame.fillTriangle(STB::WHITE, 300, 200, 100, 350, 350, 260);

   for(unsigned i = 0; i < 60; i += 3)
   {
      frame.drawSpan(STB::WHITE, 210, 140 + i, 220 + i);
   }

   frame.drawCircle(STB::CYAN, 40, 250, 20);
   frame.fillCircle(STB::CYAN, 40, 250, 15);

   frame.drawText(STB::YELLOW, 0x000000, 100, 210, &GUI::font_teletext18, "Hello, world!");

   frame.refresh();

   return PLT::Event::eventLoop();
}
