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

#include "GUI/Colour.h"
#include "GUI/Frame.h"

#include "Col.h"

namespace GUI {

class Window : public Col, public Frame
{
public:
   Window(const char* title_,
          unsigned    width_,
          unsigned    height_,
          const Font* font_,
          uint32_t    flags_)
      : Col(0, 8)
      , GUI::Frame(title_, width_, height_, flags_)
      , font(font_)
   {
   }

   //! Set event code for timer events
   void setTimerEvent(unsigned timer_code_) { timer_code = timer_code_; }

   //! External event
   void handleEvent(const PLT::Event::Message& event)
   {
      switch(event.type)
      {
      case PLT::Event::RESIZE:       winResize(event.x, event.y); break;
      case PLT::Event::KEY_DOWN:     keyPress(event.code, true ); break;
      case PLT::Event::KEY_UP:       keyPress(event.code, false); break;
      case PLT::Event::POINTER_MOVE: ptrMove(event.x, event.y); break;
      case PLT::Event::BUTTON_DOWN:  btnPress(event.x, event.y, event.code == 1, true);  break;
      case PLT::Event::BUTTON_UP:    btnPress(event.x, event.y, event.code == 1, false); break;
      case PLT::Event::TIMER:        timerEvent(); break;
      case PLT::Event::ENTER:        setHover(nullptr); break;
      case PLT::Event::LEAVE:        setHover(nullptr); break;

      case PLT::Event::QUIT:
      default:
         break;
      }
   }

   bool isHover(const Widget* that) const override { return that == hover; }

   const Font* getDefaultFont() const override { return font; }

   void addChild(Window* child)
   {
      child->next = next;
      next        = child;
   }

   Window* getNext() const { return next; }

   void show() override
   {
      layout();
      Frame::show();
   }

protected:
   Window* next{nullptr};

   //! External notification of window resize
   void winResize(unsigned width, unsigned height)
   {
      setSize(width, height);
      eventResize();
      layout();
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
      Widget* item = find(x, y);
      if (item != nullptr)
      {
         for(Widget* i = item; i != nullptr; i = i->getParent())
         {
            if (i->wantsHover())
            {
               setHover(i);
               break;
            }
         }

         if (focus != nullptr)
         {
            focus->eventPtrMove(x, y);
         }
         else
         {
            item->eventPtrMove(x, y);
         }
      }
   }

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

   //! Internal notification of an event
   void raiseEvent(Widget* focus_, unsigned code_) final override
   {
      switch(code_)
      {
      case EVENT_FOCUS:  focus = focus_;          break;
      case EVENT_REDRAW: Widget::redraw(*this);   break;
      default:           appEvent(focus_, code_); break;
      }
   }

private:
   const Font* const font{nullptr};
   unsigned          timer_code{0};
   Widget*           focus{nullptr};
   Widget*           hover{nullptr};

   void setHover(Widget* new_hover)
   {
      if (new_hover != hover)
      {
         Widget* old_hover = hover;
         hover = new_hover;

         if (old_hover != nullptr)
         {
            old_hover->redraw(*this);
         }

         if (hover != nullptr)
         {
            hover->redraw(*this);
         }
      }
   }

   // Implement Widget

   void eventResize() override { Frame::resize(Widget::size.x, Widget::size.y); }

   void eventDraw(Canvas&) override { Canvas::clear(FACE); }

   virtual void appEvent(Widget* item_, unsigned code_) {}
};

} // namespace GUI
