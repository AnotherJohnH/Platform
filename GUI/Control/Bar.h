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

#ifndef GUI_CONTROL_BAR_H
#define GUI_CONTROL_BAR_H

#include "Colour.h"
#include "Widget.h"

namespace GUI {

//! Raised bar widget
class Bar : public Widget
{
private:
   virtual void eventDraw(Canvas& canvas) override
   {
      if(isParentRow())
      {
         canvas.drawLine(LIGHT,  pos.x,     pos.y, pos.x,     pos.y + size.y - 1);
         canvas.drawLine(SHADOW, pos.x + 1, pos.y, pos.x + 1, pos.y + size.y - 1);
      }
      else
      {
         canvas.drawLine(LIGHT,  pos.x, pos.y,     pos.x + size.x - 1, pos.y);
         canvas.drawLine(SHADOW, pos.x, pos.y + 1, pos.x + size.x - 1, pos.y + 1);
      }
   }

public:
   Bar(Widget* parent)
      : Widget(parent)
   {
      if(isParentRow())
      {
         vert_fit = Fit::EXPAND;
         size.x   = 2;
      }
      else
      {
         horz_fit = Fit::EXPAND;
         size.y   = 2;
      }
   }
};

} // namespace GUI

#endif
