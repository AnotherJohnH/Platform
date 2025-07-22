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

#include "SymbolButton.h"

#include "GUI/Widget.h"

namespace GUI {

class ScrollBar : public Widget
{
public:
   ScrollBar(Widget* parent, unsigned code_, bool corner_ = false)
      : Widget(parent)
      , code(code_)
   {
      row = not parent->isRow();

      horz_fit = row ? Fit::EXPAND : Fit::SHRINK;
      vert_fit = row ? Fit::SHRINK : Fit::EXPAND;

      slider.setRow(row);

      if (corner_)
         corner.setParent(this);
   }

   //! Get current window size (0..1)
   double getWindow() const { return slider.getWindow(); }

   //! Get current offset (0..1)
   double getOffset() const { return slider.getOffset(); }

   //! Set window size (0..1)
   void setWindow(double window_) { slider.setWindow(window_); }

   //! Set offset (0..1)
   void setOffset(double offset_) { slider.setOffset(offset_); }

private:
   //! Scroll bar slider
   class ScrollSlider : public Widget
   {
   public:
      ScrollSlider(Widget* parent)
         : Widget(parent)
      {
         setExpand();
      }

      double getWindow() const { return double(window) / mux(size.x, size.y); }

      double getOffset() const { return double(offset) / mux(size.x, size.y); }

      void setWindow(double window_)
      {
         signed max = mux(size.x, size.y);

         window = window_ * max;

         if (window > max)
         {
            window = max;
         }
         else if (window < MIN_SLIDER_SIZE)
         {
            window = MIN_SLIDER_SIZE;
         }
      }

      void setOffset(double offset_)
      {
      }

      void nudge(signed dir)
      {
          changeValue(dir * window);
      }

   private:
      static const signed MIN_SLIDER_SIZE = 14;

      Vector::type offset{0};
      Vector::type window{MIN_SLIDER_SIZE};
      Vector::type ref;
      bool         select{false};

      template <typename TYPE>
      const TYPE& mux(const TYPE& row_value, const TYPE& col_value) const
      {
         return row ? row_value : col_value;
      }

      void eventDraw(Canvas& canvas) override
      {
         canvas.fillRect(LIGHT, pos.x, pos.y, pos.x + size.x, pos.y + size.y);

         Vector tl, br;
         getSliderCorners(tl, br);

         canvas.fillRect(FACE,    tl.x,     tl.y,     br.x, br.y);
         canvas.drawLine(SHADOW,  br.x - 1, br.y - 1, br.x, tl.y);
         canvas.drawLine(SHADOW,  br.x - 1, br.y - 1, tl.x, br.y);
         canvas.drawLine(HILIGHT, tl.x,     tl.y,     br.x, tl.y);
         canvas.drawLine(HILIGHT, tl.x,     tl.y,     tl.x, br.y);
      }

      void eventKeyPress(uint8_t key, bool down_) override
      {
         if(down_)
         {
            if(key == mux(PLT::LEFT, PLT::UP))
            {
               nudge(-1);
            }
            else if(key == mux(PLT::RIGHT, PLT::DOWN))
            {
               nudge(+1);
            }
         }
      }

      void eventBtnPress(signed x, signed y, bool select_, bool down_) override
      {
         if(down_)
         {
            Vector tl, br;
            getSliderCorners(tl, br);

            select = (x >= tl.x) && (x < br.x) && (y >= tl.y) && (y < br.y);
            if (select)
            {
               ref = mux(x, y);
            }
            else if (mux(x, y) < mux(tl.x, tl.y))
            {
               nudge(-1);
            }
            else if (mux(x, y) > mux(br.x, br.y))
            {
               nudge(+1);
            }

            raiseEvent(this, EVENT_FOCUS);
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
            changeValue(mux(x, y) - ref);
            ref = mux(x, y);
         }
      }

      void getSliderCorners(Vector& tl, Vector& br)
      {
         tl.x = pos.x + mux(offset, Vector::type(0));
         tl.y = pos.y + mux(Vector::type(0), offset);
         br.x = tl.x  + mux(window, size.x);
         br.y = tl.y  + mux(size.y, window);
      }

      void changeValue(signed delta)
      {
         offset += delta;
         signed max = mux(size.x, size.y);

         if (offset < 0)
            offset = 0;
         else if ((offset + window) > max)
            offset = max - window;

         raiseEvent(this, EVENT_REDRAW);
         raiseEvent(this, EV_CHANGE);
      }
   };

   void eventDraw(Canvas& canvas) override
   {
      canvas.fillRect(LIGHT,  pos.x, pos.y, pos.x + size.x, pos.y + size.y);
      canvas.drawRect(SHADOW, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
   }

   //! Intercept events from child widgets
   void raiseEvent(Widget* source_, unsigned code_) override
   {
      if (code_ == EV_LESS)
      {
         slider.nudge(-1);
      }
      else if (code_ == EV_MORE)
      {
         slider.nudge(+1);
      }
      else if (code_ == EV_CHANGE)
      {
         Widget::raiseEvent(this, code);
      }
      else
      {
         Widget::raiseEvent(source_, code_);
      }
   }

   enum Event : unsigned { EV_LESS = 1, EV_MORE, EV_CHANGE, EV_RESIZE };

   const unsigned code;
   SymbolButton   btn_scroll_less{this, EV_LESS, isParentRow() ? Symbol::LEFT
                                                               : Symbol::UP};
   ScrollSlider   slider{this};
   SymbolButton   btn_scroll_more{this, EV_MORE, isParentRow() ? Symbol::RIGHT
                                                               : Symbol::DOWN};
   Symbol         corner{nullptr, Symbol::CORNER, 19};
};

} // namespace GUI
