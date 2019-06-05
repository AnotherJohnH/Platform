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

#ifndef GUI_CONTROL_MENU_BUTTON_H
#define GUI_CONTROL_MENU_BUTTON_H

#include "Colour.h"
#include "Row.h"
#include "Text.h"

namespace GUI {

//! Button base class
class MenuButton : public Row 
{
public:
   MenuButton(Widget* parent_, unsigned code_, const char* text_)
      : Row(parent_)
      , code(code_)
      , text(this, text_)
   {
      horz_fit = Fit::EXPAND;
      setBorderAndGap(4);
   }

protected:
   virtual void eventDraw(Canvas& canvas) override
   {
      STB::Colour fg_colour = isHover(this) ? STB::WHITE : STB::BLACK;
      STB::Colour bg_colour = isHover(this) ? STB::BLUE  : FACE;

      canvas.fillRect(bg_colour, pos.x, pos.y, pos.x + size.x, pos.y + size.y);

      text.setForegroundColour(fg_colour);
      text.setBackgroundColour(bg_colour);
   }

   virtual void eventBtnPress(signed x, signed y, bool select_, bool down_) override
   {
      if (down_)
      {
         raiseEvent(this, EVENT_FOCUS);
      }
      else if((code != 0) && !down_ && isHit(x, y))
      {
         raiseEvent(this, code);
      }
   }

   virtual bool wantsHover() const override
   {
      return code != 0;
   }

private:
   uint32_t code{0};
   Text     text;
};

} // namespace GUI

#endif
