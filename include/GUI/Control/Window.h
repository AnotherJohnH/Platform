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

#ifndef GUI_CONTROL_WINDOW_H
#define GUI_CONTROL_WINDOW_H

#include "GUI/Frame.h"

#include "Col.h"
#include "Colour.h"

namespace GUI {

class Window : public Col, public Frame
{
private:
   const Font* const font{nullptr};
   unsigned          timer_code{0};
   Widget*           focus{nullptr};

   // Implement Widget

   virtual void eventResize() override { Frame::resize(Widget::size.x, Widget::size.y); }

   virtual void eventDraw(Canvas&) override { Canvas::clear(FACE); }

   virtual const Font* getDefaultFont() const override { return font; }

   virtual void appEvent(Widget* item_, unsigned code_) {}

protected:
   virtual void raiseEvent(Widget* focus_, unsigned code_) final override
   {
      switch(code_)
      {
      case EVENT_FOCUS:  focus = focus_;          break;
      case EVENT_REDRAW: Widget::redraw(*this);   break;
      default:           appEvent(focus_, code_); break;
      }
   }

public:
   Window(const char* title_, unsigned width_, unsigned height_, const Font* font_, uint32_t flags_)
      : Col(0, 8)
      , GUI::Frame(title_, width_, height_, flags_)
      , font(font_)
   {
   }

   //! Set event code for timer events
   void setTimerEvent(unsigned timer_code_) { timer_code = timer_code_; }

   //! External notification of window resize
   void winResize(unsigned width, unsigned height)
   {
      setSize(width, height);
      eventResize();
      show();
   }

   //! External notification of a timer event
   void timerEvent()
   {
      if(timer_code)
      {
         raiseEvent(this, timer_code);
         raiseEvent(this, EVENT_REDRAW);
      }
   }

   //! External notification of a pointer move
   void ptrMove(unsigned x, unsigned y)
   {
      Widget* item = focus ? focus : find(x, y);
      if(item)
      {
         item->eventPtrMove(x, y);
      }
   }

   //! External notification of a button press
   void btnPress(unsigned x, unsigned y, bool select, bool down)
   {
      Widget* hit = down ? find(x, y) : focus;
      if(hit)
      {
         hit->eventBtnPress(x, y, select, down);
      }
   }

   //! External notification of a key press
   void keyPress(uint8_t key, bool down)
   {
      if(focus)
      {
         focus->eventKeyPress(key, down);
      }
   }
};

} // namespace GUI

#endif
