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

#include "PLT/AudioOut.h"


#include  "PLT/Paper.h"
#include  "PLT/Event.h"

#include  "GUI/Font/Teletext18.h"


int main(int argc, const char* argv[])
{
   PLT::Paper paper("Paper Test", 400, 400);

   paper.clear(GUI::BLACK);

   paper.drawPoint(GUI::WHITE, 5, 5);

   paper.fillRect(GUI::WHITE, 10,  10,  40,  40);
   paper.drawRect(GUI::WHITE, 10, 110,  40, 140);

   paper.fillRect(GUI::RED, 110,  10, 140,  40);
   paper.drawRect(GUI::RED, 110, 110, 140, 140);

   paper.fillRect(GUI::GREEN, 210,  10, 240,  40);
   paper.drawRect(GUI::GREEN, 210, 110, 240, 140);

   paper.fillRect(GUI::BLUE, 310,  10, 340,  40);
   paper.drawRect(GUI::BLUE, 310, 110, 340, 140);

   paper.drawLine(GUI::MAGENTA, 10, 200, 60, 250);
   paper.drawLine(GUI::MAGENTA, 60, 200, 10, 250);

   paper.fillTriangle(GUI::WHITE,
                      300, 200,
                      100, 350,
                      350, 260);

   paper.drawCircle(GUI::CYAN, 100, 300, 20);
   paper.fillCircle(GUI::CYAN, 100, 300, 15);

   paper.drawText(GUI::YELLOW, 0x000000, 120, 200, &GUI::font_teletext18,
                  "Hello, world!");

   paper.refresh();

   PLT::Event  event;
   while(PLT::waitEvent(event) != PLT::QUIT);

   return 0;
}

