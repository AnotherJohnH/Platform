//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

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

   unsigned timer_code{0};
};

} // namespace GUI
