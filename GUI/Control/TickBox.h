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

#pragma once

#include "Row.h"

namespace GUI {

//! TickBox base class
class TickBox : public Widget
{
public:
   TickBox(Widget* parent, unsigned code_)
      : Widget(parent)
      , code(code_)
   {
      size.x = SIZE;
      size.y = SIZE;
   }

protected:
   // Implement Widget events

   void eventDraw(Canvas& canvas) override
   {
      canvas.fillRect(BACKGROUND, pos.x, pos.y, pos.x + size.x, pos.y + size.y);

      if (set)
      {
         canvas.drawLine(FOREGROUND, pos.x, pos.y,     pos.x + size.x, pos.y + size.y);
         canvas.drawLine(FOREGROUND, pos.x, pos.y + 1, pos.x + size.x - 1, pos.y + size.y);

         canvas.drawLine(FOREGROUND, pos.x,     pos.y + size.y, pos.x + size.x, pos.y);
         canvas.drawLine(FOREGROUND, pos.x + 1, pos.y + size.y, pos.x + size.x, pos.y + 1);
      }

      canvas.drawLine(SHADOW,  pos.x, pos.y, pos.x + size.x, pos.y);
      canvas.drawLine(SHADOW,  pos.x, pos.y, pos.x, pos.y + size.y);
      canvas.drawLine(HILIGHT, pos.x + size.x, pos.y + size.y, pos.x + size.x, pos.y);
      canvas.drawLine(HILIGHT, pos.x + size.x, pos.y + size.y, pos.x, pos.y + size.y);

      Widget::eventDraw(canvas);
   }

   void eventBtnPress(signed x, signed y, bool select, bool down_) override
   {
      operate(down_, isHit(x, y));
   }

   void eventKeyPress(uint8_t key, bool down_) override
   {
      if(key == ' ')
      {
         operate(down_, true);
      }
   }

private:
   void operate(bool down_, bool over_)
   {
      if(down_)
      {
         raiseEvent(this, EVENT_FOCUS);
      }
      else
      {
         if(over_)
         {
            set = !set;
            raiseEvent(this, code);
         }
      }

      raiseEvent(this, EVENT_REDRAW);
   }

   static const unsigned SIZE = 12;

   const unsigned code{};
   bool           set{false};
};

} // namespace GUI
