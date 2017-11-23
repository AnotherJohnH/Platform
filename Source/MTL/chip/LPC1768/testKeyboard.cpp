//------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
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

// \brief 

#include "GUI/Frame.h"
#include "GUI/Font/Teletext.h"
#include "Keyboard.h"

int main()
{
   GUI::Frame     frame("", 400, 300);
   MTL::Keyboard  keyboard;

   while(true)
   {
      bool    up;
      uint8_t ch;
      
      if (keyboard.recv(ch, up))
      {
         char buffer[3];

         buffer[0] = ch;
         buffer[1] = up ? '^' : 'v';
         buffer[2] = '\0';

         frame.fillRect(GUI::BLACK, 0, 0, 30, 20);
         frame.drawText(GUI::WHITE, 0x000000, 0, 0, &GUI::font_teletext18, buffer);
      }
   }
}
