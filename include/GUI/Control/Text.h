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

#ifndef GUI_CONTROL_TEXT_H
#define GUI_CONTROL_TEXT_H

#include <string>

#include "GUI/Widget.h"

namespace GUI {

class Text : public Widget
{
private:
   std::string text;

   // Implement Widget events
   virtual void eventDraw(Canvas& canvas) override
   {
      canvas.drawText(fg_colour, bg_colour, pos.x, pos.y, font, text.c_str());
   }

public:
   Text(Widget* parent, const std::string& text_)
      : Widget(parent)
      , text(text_)
   {
      size.x           = font->getWidth(text.c_str());
      size.y           = font->getHeight();
   }

   template <typename TYPE>
   void setText(TYPE text_) { text = text_; }

   void setWidth(unsigned width_)
   {
      size.x = font->getWidth(" ") * width_;
   }
};

} // namespace GUI

#endif
