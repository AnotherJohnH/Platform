//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace GUI {

class Symbol : public Widget
{
public:
   enum Type { LEFT, RIGHT, UP, DOWN, CORNER };

   Symbol(Widget*  parent_,
          Type     type_,
          unsigned size_ = 12)
      : Widget(parent_)
      , type(type_)
   {
      setSize(size_, size_);
   }

protected:
   void eventDraw(Canvas& canvas) override
   {
      switch(type)
      {
      case LEFT:
      case RIGHT:
         {
            signed yt = pos.y;
            signed yb = pos.y + size.y;
            if (type == RIGHT) std::swap(yt, yb);

            canvas.fillTriangle(GUI::FOREGROUND,
                                pos.x + size.x/2, yt,
                                pos.x,            yb,
                                pos.x + size.x,   yb);
         }
         break;

      case UP:
      case DOWN:
         {
            signed xt = pos.x;
            signed xb = pos.x + size.x;
            if (type == DOWN) std::swap(xt, xb);

            canvas.fillTriangle(GUI::FOREGROUND,
                                xt, pos.y + size.y/2,
                                xb, pos.y,
                                xb, pos.y + size.y);
         }
         break;

      case CORNER:
         break;
      }
   }

private:
   Type type;
};

} // namespace GUI
