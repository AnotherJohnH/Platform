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

#ifndef GUI_CONTROL_BUTTON_H
#define GUI_CONTROL_BUTTON_H

#include "Colour.h"
#include "Row.h"

namespace GUI {

//! Button base class
class Button : public Row
{
private:
   STB::Colour  bg_colour{FACE};
   uint32_t     code{0};
   uint32_t     alt_code{0};
   bool         down{false};
   bool         select{false};
   bool         flat{false};

   void operate(bool down_, bool over_, bool select_)
   {
      if(down != down_)
      {
         down = down_;
         if(down)
         {
            nudgeChildren(+1, +1);
            raiseEvent(this, EVENT_FOCUS);
         }
         else
         {
            nudgeChildren(-1, -1);

            unsigned c = select_ ? code : alt_code;
            if(c && over_)
            {
               raiseEvent(this, c);
            }
         }

         layout();
      }
   }

protected:
   // Implement Widget events

   virtual void eventDraw(Canvas& canvas) override
   {
      STB::Colour tl = (down || select) ? SHADOW  : HILIGHT;
      STB::Colour br = (down || select) ? HILIGHT : SHADOW;

      canvas.fillRect(bg_colour, pos.x, pos.y, pos.x + size.x, pos.y + size.y);

      if(!flat || down)
      {
         canvas.drawLine(tl, pos.x, pos.y, pos.x + size.x - 1, pos.y);
         canvas.drawLine(tl, pos.x, pos.y, pos.x, pos.y + size.y - 1);
         canvas.drawLine(br, pos.x + size.x - 1, pos.y + size.y -1, pos.x + size.x - 1, pos.y);
         canvas.drawLine(br, pos.x + size.x - 1, pos.y + size.y -1, pos.x, pos.y + size.y - 1);
      }

      Widget::eventDraw(canvas);
   }

   virtual void eventBtnPress(signed x, signed y, bool select_, bool down_) override
   {
      operate(down_, isHit(x, y), select_);
   }

   virtual void eventKeyPress(uint8_t key, bool down_) override
   {
      if(key == ' ')
      {
         operate(down_, true, true);
      }
   }

public:
   Button()
   {
      Row::setBorderAndGap(4);
   }

   Button(Widget* parent, unsigned code_)
   {
      init(parent, code_);
   }

   void init(Widget* parent, unsigned code_)
   {
      Row::init(parent, 4);
      code = code_;
   }

   void setCode(unsigned code_) { code = code_; }

   void setSelect(bool set = true) { select = set; }

   void setFlat(bool set = true) { flat = set; }

   void setAltCode(bool select, unsigned code_) { alt_code = code_; }

   void setBackgroundColour(STB::Colour colour) { bg_colour = colour; }
};

} // namespace GUI

#endif
