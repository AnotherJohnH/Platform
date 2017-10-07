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

#include  "PLT/Canvas.h"
#include  "PLT/Event.h"
#include  "GUI/GUI.h"

int main(int argc, const char* argv[])
{
   PLT::Canvas canvas("Canvas Test", 400, 300);

   canvas.clear(GUI::BLACK);

   canvas.fillRect(GUI::RED,         10,  10,  42,  42);
   canvas.fillRect(GUI::GREEN,       42,  10,  74,  42);
   canvas.fillRect(GUI::YELLOW,      74,  10, 106,  42);
   canvas.fillRect(GUI::BLUE,       106,  10, 138,  42);
   canvas.fillRect(GUI::MAGENTA,    138,  10, 170,  42);
   canvas.fillRect(GUI::CYAN,       170,  10, 202,  42);
   canvas.fillRect(GUI::WHITE,      202,  10, 234,  42);

   canvas.fillRect(GUI::BACKGROUND,  10,  50,  42,  82);
   canvas.fillRect(GUI::FOREGROUND,  42,  50,  74,  82);
   canvas.fillRect(GUI::DARK,        74,  50, 106,  82);
   canvas.fillRect(GUI::SHADOW,     106,  50, 138,  82);
   canvas.fillRect(GUI::FACE,       138,  50, 170,  82);
   canvas.fillRect(GUI::LIGHT,      170,  50, 202,  82);
   canvas.fillRect(GUI::HILIGHT,    202,  50, 234,  82);

   for(unsigned x=0; x<256; x++)
   {
      canvas.drawLine(GUI::GREY(x), 10+x, 90, 10+x, 122);
   }

   for(unsigned x=0; x<256; x += 4)
   {
      canvas.drawPoint(GUI::WHITE, 10+x, 130);
   }

   canvas.fillRect(GUI::WHITE,   10,  140, 70, 200);
   canvas.drawRect(GUI::BLACK,   11,  141, 68, 198);
   canvas.drawRect(GUI::BLACK,   14,  144, 65, 195);

   for(unsigned i=0; i<=60; i += 10)
   {
      canvas.drawLine(GUI::WHITE, 140, 170, 110 + i, 140);
      canvas.drawLine(GUI::WHITE, 140, 170, 170,     140 + i);
      canvas.drawLine(GUI::WHITE, 140, 170, 110 + i, 200);
      canvas.drawLine(GUI::WHITE, 140, 170, 110,     140 + i);
   }

   canvas.fillTriangle(GUI::WHITE,
                      300, 200,
                      100, 350,
                      350, 260);

   for(unsigned i=0; i<60; i += 3)
   {
      canvas.drawSpan(GUI::WHITE, 210, 140 + i, 220 + i);
   }

   canvas.drawCircle(GUI::CYAN, 40, 250, 20);
   canvas.fillCircle(GUI::CYAN, 40, 250, 15);

   canvas.drawText(GUI::YELLOW, 0x000000, 100, 210, &GUI::font_teletext18,
                  "Hello, world!");

   canvas.refresh();

   PLT::Event  event;
   while(PLT::waitEvent(event) != PLT::QUIT);

   return 0;
}

