//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "GUI/Colour.h"
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
   void eventDraw(Canvas& canvas) override
   {
      STB::Colour fg_colour = isHover(this) ? STB::WHITE : STB::BLACK;
      STB::Colour bg_colour = isHover(this) ? STB::BLUE  : FACE;

      canvas.fillRect(bg_colour, pos.x, pos.y, pos.x + size.x, pos.y + size.y);

      text.setForegroundColour(fg_colour);
      text.setBackgroundColour(bg_colour);
   }

   void eventBtnPress(signed x, signed y, bool select_, bool down_) override
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

   bool wantsHover() const override
   {
      return code != 0;
   }

private:
   uint32_t code{0};
   Text     text;
};

} // namespace GUI
