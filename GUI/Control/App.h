//------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
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

#ifndef GUI_APP_H
#define GUI_APP_H

#include "PLT/Event.h"

#include "GUI/Control/Window.h"

namespace GUI {

class App : public Window
{
public:
   App(const char* title_, const GUI::Font* font_, uint32_t flags_ = 0)
      : GUI::Window(title_, 0, 0, font_, flags_)
   {
   }

   App(const char*      title_,
       const GUI::Font* font_,
       unsigned         width_,
       unsigned         height_,
       uint32_t         flags_ = 0)
      : GUI::Window(title_, width_, height_, font_, flags_)
   {
      setSize(width_, height_);
   }

   int eventLoop()
   {
      layout();

      return PLT::Event::eventLoop(eventCallBack, this);
   }

   void setTimer(unsigned code, unsigned period_ms)
   {
      timer_code = code;
      setTimerEvent(code);
      PLT::Event::setTimer(period_ms);
   }

private:
   unsigned timer_code{0};

   //! Callback from PLT::Event
   static void eventCallBack(const PLT::Event::Message& event, void* ptr)
   {
      App* app = reinterpret_cast<App*>(ptr);

      for(Window* win = app->next; win != nullptr; win = win->getNext())
      {
         if (win->getId() == event.window)
         {
            win->handleEvent(event);
            return;
         }
      }

      app->handleEvent(event);
   }
};

} // namespace GUI

#endif // GUI_APP_H
