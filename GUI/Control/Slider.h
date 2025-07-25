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

class Slider : public Widget
{
public:
   Slider(Widget*  parent_,
          unsigned code_,
          unsigned length_ = LENGTH,
          bool     vertical_ = true)
      : Widget(parent_)
      , code(code_)
      , length(length_)
      , vertical(vertical_)
   {
      if (vertical)
         setSize(BORDER * 2, BORDER * 2 + length);
      else
         setSize(BORDER * 2 + length, BORDER * 2);
   }

   double getValue() const { return double(value) / length; }

   void setValue(double value_) { changeValue(value_ * length); }

private:
   void transform(Vector& p, int x, int y)
   {
      p.x = pos.x + BORDER;
      p.y = pos.y + BORDER;

      if(vertical)
      {
         p.x += x;
         p.y += length - y;
      }
      else
      {
         p.x += length - y;
         p.y += x;
      }
   }

   void line(Canvas& canvas, STB::Colour col, int x1, double y1, int x2, double y2)
   {
      Vector from;
      Vector to;

      transform(from, x1, y1);
      transform(to,   x2, y2);

      canvas.drawLine(col, from.x, from.y, to.x, to.y);
   }

   void changeValue(int value_)
   {
      value = value_;

      if(value < 0)
         value = 0;
      else if(value > int(length))
         value = length;

      raiseEvent(this, code);
      raiseEvent(this, EVENT_REDRAW);
   }


   // Implement Widget events

   void eventDraw(Canvas& canvas) override
   {
      line(canvas, DARK, 0, 0, 0, length);

      line(canvas, DARK, -4, length, +5,        length);
      line(canvas, DARK, -1, length * 0.75, +2, length * 0.75);
      line(canvas, DARK, -3, length * 0.5,  +4, length * 0.50);
      line(canvas, DARK, -1, length * 0.25, +2, length * 0.25);
      line(canvas, DARK, -4, 0,             +5, 0);

      Vector v;
      transform(v, 0, value);

      canvas.fillRect(SHADOW, v.x - KNOB, v.y - KNOB, v.x + KNOB, v.y + KNOB);

      canvas.drawLine(LIGHT, v.x - KNOB, v.y - KNOB, v.x + KNOB, v.y - KNOB);
      canvas.drawLine(LIGHT, v.x - KNOB, v.y - KNOB, v.x - KNOB, v.y + KNOB);
      canvas.drawLine(DARK,  v.x + KNOB, v.y + KNOB, v.x + KNOB, v.y - KNOB);
      canvas.drawLine(DARK,  v.x + KNOB, v.y + KNOB, v.x - KNOB, v.y + KNOB);
   }

   void eventBtnPress(signed x, signed y, bool select_, bool down_) override
   {
      if(down_)
      {
         Vector v;

         transform(v, 0, value);

         select = (x >= v.x - KNOB) && (x <= v.x + KNOB) &&
                  (y >= v.y - KNOB) && (y <= v.y + KNOB);
         if (select)
         {
            ref       = vertical ? y : x;
            ref_value = value;
            raiseEvent(this, EVENT_FOCUS);
         }
      }
      else
      {
         select = false;
      }
   }

   void eventPtrMove(unsigned x, unsigned y) override
   {
      if(select)
      {
         int delta = ref - (vertical ? y : x);

         changeValue(ref_value + delta);
      }
   }

   static const signed   KNOB   = 8;
   static const unsigned BORDER = 16;
   static const unsigned LENGTH = 100;

   unsigned code;
   unsigned length;
   bool     vertical;
   int      value{0};
   bool     select{false};
   int      ref_value{0};
   unsigned ref{0};
};

} // namespace GUI
