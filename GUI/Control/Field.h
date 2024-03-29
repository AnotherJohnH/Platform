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

#ifndef GUI_CONTROL_FIELD_H
#define GUI_CONTROL_FIELD_H

#include <cstring>

#include "PLT/Event.h"

#include "Colour.h"
#include "Widget.h"

namespace GUI {

template <unsigned COLS>
class Field : public Widget, public ColourPair
{
public:
   Field() = default;

   Field(Widget* parent_, unsigned code_ = 0, const char* initial_ = "")
   {
      init(parent_, code_, initial_);
   }
 
   void init(Widget* parent_, unsigned code_, const char* initial_)
   {
      setParent(parent_);

      // code = code_;

      value[COLS] = '\0';
      setValue(initial_);

      setBackgroundColour(BACKGROUND);
      setForegroundColour(FOREGROUND);

      eventSize();
   }

   void setValue(const char* value_)
   {
      // XXX Avoid a gcc warning with...
      // strncpy(value, value_, COLS); 
      for(unsigned i = 0; i < COLS; ++i)
      {
         value[i] = value_[i];
         if (value[i] == '\0')
             break;
      }
      value[COLS] = '\0';
   }

protected:
   // Implement Widget events
   virtual void eventSize() override
   {
      if (font == nullptr)
      {
         font = getDefaultFont();
      }

      setBorderAndGap(2);
      size.x = font->getWidth() * COLS + 4;
      size.y = font->getHeight() + 4;
   }

   virtual void eventDraw(Canvas& canvas) override
   {
      canvas.fillRect(bg_colour, pos.x, pos.y, pos.x + size.x, pos.y + size.y);

      canvas.drawText(fg_colour, FACE, pos.x + top_left.x, top_left.y + pos.y, font,
                      value);
   }

   virtual void eventBtnPress(signed x, signed y, bool select, bool down_) override
   {
      if(down_)
      {
         raiseEvent(this, EVENT_FOCUS);
      }
   }

   virtual void eventKeyPress(uint8_t key, bool down_) override
   {
      if(down_)
      {
         unsigned n = strlen(value);

         switch(key)
         {
         case PLT::LSHIFT:
         case PLT::RSHIFT:
            break;

         case PLT::BACKSPACE:
            if(n > 0)
            {
               value[n - 1] = '\0';
            }
            break;

         default:
            if(n < COLS)
            {
               value[n] = key;
            }
            break;
         }

         raiseEvent(this, EVENT_REDRAW);
      }
   }

private:
   const Font* font{nullptr};
   // unsigned      code;
   char        value[COLS + 1];
};

} // namespace GUI

#endif
