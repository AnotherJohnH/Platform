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

#include "GUI/GUI.h"


class PopUp : public GUI::Frame
{
private:
   GUI::Text text;

public:
   PopUp()
      : GUI::Frame("Pop Up", &GUI::font_teletext18)
      , text(this, "Hello, world!")
   {}
};


class MainWindow : public GUI::Frame
{
private:
   PopUp popup;

   GUI::Text        text2;
   GUI::Button      button;
   GUI::TextButton  txt_btn;
   GUI::TextButton  btn_a;
   GUI::TextButton  btn_b;
   GUI::TickBox     tick;
   GUI::TextTickBox text_tick;
   GUI::Field<16>   field;

   virtual void appEvent(Widget* widget_, unsigned code_) override
   {
      printf("raiseEvent(%u)\n", code_);

      if(code_ == 'B') popup.show();
   }

public:
   MainWindow()
      : GUI::Frame("GUI test", &GUI::font_teletext18)
      , text2(this, "Hello, world!")
      , button(this, 1)
      , txt_btn(this, 2, "0123456789")
      , btn_a(this, 'A', "Cancel")
      , btn_b(this, 'B', "OK")
      , tick(this, 3)
      , text_tick(this, 3, "Do you agree?")
      , field(this, 4, "")
   {}
};


int main( int argc, char *argv[] )
{
   MainWindow().eventLoop();
}

