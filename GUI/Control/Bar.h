//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "GUI/Colour.h"
#include "GUI/Widget.h"

namespace GUI {

//! Raised bar widget
class Bar : public Widget
{
public:
   Bar(Widget* parent_= nullptr)
      : Widget(parent_)
   {
   }

protected:
   void eventLayout() override
   {
      if (isParentRow())
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

   void eventDraw(Canvas& canvas_) override
   {
      if(isParentRow())
      {
         canvas_.drawLine(LIGHT,  pos.x,     pos.y, pos.x,     pos.y + size.y - 1);
         canvas_.drawLine(SHADOW, pos.x + 1, pos.y, pos.x + 1, pos.y + size.y - 1);
      }
      else
      {
         canvas_.drawLine(LIGHT,  pos.x, pos.y,     pos.x + size.x - 1, pos.y);
         canvas_.drawLine(SHADOW, pos.x, pos.y + 1, pos.x + size.x - 1, pos.y + 1);
      }
   }
};

} // namespace GUI
