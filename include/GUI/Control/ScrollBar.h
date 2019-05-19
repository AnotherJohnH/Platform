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

#ifndef GUI_CONTROL_SCROLL_BAR_H
#define GUI_CONTROL_SCROLL_BAR_H

#include "Expand.h"
#include "TextButton.h"
#include "Widget.h"

namespace GUI {

class ScrollBar : public Widget
{
private:
   static const unsigned LESS = 1;
   static const unsigned MORE = 2;

   class Symbol : public Widget
   {
   public:
      Symbol(Widget* parent_, unsigned code_, bool vertical_)
         : Widget(parent_)
         , code(code_)
      {
         setSize(14, 14);
         row = !vertical_;
      }

   private:
      unsigned code{};

      virtual void eventDraw(Canvas& canvas) override
      {
         if (isRow())
         {
            if (code == MORE)
            {
               canvas.fillTriangle(GUI::FOREGROUND,
                                   pos.x + size.x, pos.y + size.y/2,
                                   pos.x,          pos.y,
                                   pos.x,          pos.y + size.y);
            }
            else
            {
               canvas.fillTriangle(GUI::FOREGROUND,
                                   pos.x         , pos.y + size.y/2,
                                   pos.x + size.x, pos.y,
                                   pos.x + size.x, pos.y + size.y);
            }
         }
         else
         {
            if (code == MORE)
            {
               canvas.fillTriangle(GUI::FOREGROUND,
                                   pos.x + size.x/2, pos.y + size.y,
                                   pos.x,            pos.y,
                                   pos.x + size.x,   pos.y);
            }
            else
            {
               canvas.fillTriangle(GUI::FOREGROUND,
                                   pos.x + size.x/2, pos.y,
                                   pos.x,            pos.y + size.y,
                                   pos.x + size.x,   pos.y + size.y);
            }
         }
      }
   };

   class ScrollButton : public Button
   {
   private:
      Symbol symbol;

   public:
      ScrollButton(Widget* parent_, unsigned code_)
         : Button(parent_, code_)
         , symbol(this, code_, parent->isParentRow())
      {
      }
   };

   class ScrollSlider : public Widget
   {
   public:
      ScrollSlider(Widget* parent)
         : Widget(parent)
      {
         setExpand();
         row = parent->isParentRow();
      }

   private:
      // unsigned pos{0};

      virtual void eventDraw(Canvas& canvas) override
      {
      }
   };

   // const unsigned  code;
   ScrollButton  btn_scroll_less{this, LESS};
   ScrollSlider  spacer{this};
   ScrollButton  btn_scroll_more{this, MORE};

   virtual void eventDraw(Canvas& canvas) override
   {
      canvas.fillRect(LIGHT,  pos.x, pos.y, pos.x + size.x, pos.y + size.y);
      canvas.drawRect(SHADOW, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
   }

public:
   ScrollBar(Widget* parent, unsigned code_)
      : Widget(parent)
      //, code(code_)
   {
      if (!parent->isRow())
      {
         row = true;
         horz_fit = Fit::EXPAND;
         vert_fit = Fit::SHRINK;
      }
      else
      {
         row = false;
         horz_fit = Fit::SHRINK;
         vert_fit = Fit::EXPAND;
      }
   }
};

} // namespace GUI

#endif
