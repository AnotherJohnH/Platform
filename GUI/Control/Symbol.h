//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
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

private:
   virtual void eventDraw(Canvas& canvas) override
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

   Type type;
};

} // namespace GUI
