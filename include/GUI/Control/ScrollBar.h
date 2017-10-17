//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#ifndef GUI_CONTROL_SCROLL_BAR_H
#define GUI_CONTROL_SCROLL_BAR_H

#include "GUI/Control/Expand.h"
#include "GUI/Control/TextButton.h"
#include "GUI/Widget.h"

namespace GUI {

class ScrollBar : public Widget
{
private:
   static const unsigned LESS = 1;
   static const unsigned MORE = 2;

   // const unsigned  code;
   TextButton btn_scroll_less;
   Expand     spacer;
   TextButton btn_scroll_more;

   virtual void eventDraw(Canvas& canvas) override
   {
      canvas.fillRect(LIGHT,  pos.x, pos.y, pos.x + size.x, pos.y + size.y);
      canvas.drawRect(SHADOW, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
   }

public:
   ScrollBar(Widget* parent, unsigned code_)
      : Widget(parent)
      //, code(code_)
      , btn_scroll_less(this, LESS, "<")
      , spacer(this)
      , btn_scroll_more(this, MORE, ">")
   {
      row = !parent->isRow();
      setShrink();
   }
};

} // namespace GUI

#endif
