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

#include <cstring>

#include "GUI/Widget.h"

namespace GUI {

class Text : public Widget, public ColourPair
{
public:
   Text(Widget* parent_ = nullptr)
      : Widget(parent_)
   {
   }

   Text(Widget* parent_, const char* text_)
      : Widget(parent_)
   {
      setText(text_);
   }

   void setFont(const Font* font_)
   {
      font = font_;
      if (font == nullptr)
      {
         font = getDefaultFont();
      }
   }

   void setText(const char* text_)
   {
      text = text_;
      len  = strlen(text);

      if (cols == 0)
         cols = len;
   }

   void setCols(unsigned cols_)
   {
      cols = cols_;
   }

   void setAlign(Align text_align_)
   {
      text_align = text_align_;
   }

protected:
   // Implement Widget events
   void eventLayout() override
   {
      if (font == nullptr)
      {
         font = getDefaultFont();
      }

      size.x = font->getWidth(" ") * cols;
      size.y = font->getHeight();
   }

   void eventDraw(Canvas& canvas) override
   {
      signed x = pos.x;

      switch(text_align)
      {
      case Align::LEFT:   break;
      case Align::CENTER: x += (cols - len) * font->getWidth(" ")/2; break;
      case Align::RIGHT:  x += (cols - len) * font->getWidth(" ");   break;
      }

      canvas.fillRect(bg_colour, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
      canvas.drawText(fg_colour, bg_colour, x, pos.y, font, text);
   }

private:
   const Font* font{nullptr};
   const char* text{nullptr};
   uint8_t     len{0};
   uint8_t     cols{0};
   Align       text_align{Align::LEFT};
};

} // namespace GUI

