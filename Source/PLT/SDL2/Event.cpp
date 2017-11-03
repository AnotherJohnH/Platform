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

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "SDL_headers.h"

#include "PLT/Event.h"

#ifdef PROJ_TARGET_Emscripten
#include "emscripten.h"
#endif


//! Call-back used by PLT::Event::eventLoop()
static void (*event_callback)(const PLT::Event::Message&, void*) = nullptr;

//! Call-back used by PLT::Event::mainLoop()
static bool (*main_callback)(void*) = nullptr;

//! User data for use with PLT::Event::eventLoop() or PLT::Event::mainLoop()
static void* user_data              = nullptr;


//! Call back used with SDL_AddTimer()
static Uint32 timer_callback(Uint32 period_ms, void* param)
{
   SDL_Event     event;
   SDL_UserEvent user_event;

   memset(&user_event, 0, sizeof(user_event));

   user_event.type = SDL_USEREVENT;

   event.type = SDL_USEREVENT;
   event.user = user_event;

   SDL_PushEvent(&event);

   return period_ms;
}


//! Translate SDL key codes to PLT key codes
static uint8_t translate_key(SDL_Keycode key)
{
   switch(key)
   {
   case SDLK_RSHIFT:    return PLT::RSHIFT;
   case SDLK_LSHIFT:    return PLT::LSHIFT;
   case SDLK_LCTRL:     return PLT::LCTRL;
   case SDLK_RALT:      return PLT::RALT;
   case SDLK_LALT:      return PLT::LALT;
   case SDLK_F1:        return PLT::F1;
   case SDLK_F2:        return PLT::F2;
   case SDLK_F3:        return PLT::F3;
   case SDLK_F4:        return PLT::F4;
   case SDLK_F5:        return PLT::F5;
   case SDLK_F6:        return PLT::F6;
   case SDLK_F7:        return PLT::F7;
   case SDLK_F8:        return PLT::F8;
   case SDLK_F9:        return PLT::F9;
   case SDLK_F10:       return PLT::F10;
   case SDLK_F11:       return PLT::F11;
   case SDLK_F12:       return PLT::F12;
   case SDLK_CAPSLOCK:  return PLT::CAPSLOCK;
   case SDLK_UP:        return PLT::UP;
   case SDLK_DOWN:      return PLT::DOWN;
   case SDLK_RIGHT:     return PLT::RIGHT;
   case SDLK_LEFT:      return PLT::LEFT;
   case SDLK_BACKSPACE: return PLT::BACKSPACE;
   case SDLK_RETURN:    return PLT::RETURN;
   case 0xA7:           return '`';
   default:             return key;
   }
}


static bool loopIter()
{
   return main_callback != nullptr ? (*main_callback)(user_data)
                                   : true;
}


static bool eventLoopIter(void* user_data_)
{
   PLT::Event::Message event;
   PLT::Event::Type    type = PLT::Event::wait(event);

   if(type != PLT::Event::NONE)
   {
      if(event_callback != nullptr) (*event_callback)(event, user_data_);
   }

   return type != PLT::Event::QUIT;
}


#ifdef PROJ_TARGET_Emscripten
static void wrapLoopIter()
{
   (void)loopIter();
}
#endif


namespace PLT {

static Event::Type getEvent(Event::Message& event, bool wait)
{
   event.type = Event::NONE;
   event.code = 0;
   event.x    = 0;
   event.y    = 0;

   SDL_Event sdl_event;

   if(wait ? SDL_WaitEvent(&sdl_event)
           : SDL_PollEvent(&sdl_event))
   {
      switch(sdl_event.type)
      {
      case SDL_QUIT:
         event.type = Event::QUIT;
         break;

      case SDL_WINDOWEVENT:
         event.code = sdl_event.window.windowID;
         switch(sdl_event.window.event)
         {
         case SDL_WINDOWEVENT_CLOSE:
            event.type = Event::QUIT;
            break;

         case SDL_WINDOWEVENT_RESIZED:
            event.type = Event::RESIZE;
            event.x    = sdl_event.window.data1;
            event.y    = sdl_event.window.data2;
            break;

         default:
            break;
         }
         break;

      case SDL_KEYDOWN:
         event.type = Event::KEY_DOWN;
         event.code = translate_key(sdl_event.key.keysym.sym);
         break;

      case SDL_KEYUP:
         event.type = Event::KEY_UP;
         event.code = translate_key(sdl_event.key.keysym.sym);
         break;

      case SDL_MOUSEMOTION:
         event.type = Event::POINTER_MOVE;
         event.x    = sdl_event.motion.x;
         event.y    = sdl_event.motion.y;
         break;

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
         event.type = sdl_event.button.type == SDL_MOUSEBUTTONUP ? Event::BUTTON_UP
                                                                 : Event::BUTTON_DOWN;
         switch(sdl_event.button.button)
         {
         case SDL_BUTTON_LEFT:  event.code = uint8_t(Event::Button::LEFT);  break;
         case SDL_BUTTON_RIGHT: event.code = uint8_t(Event::Button::RIGHT); break;
         }
         event.x = sdl_event.button.x;
         event.y = sdl_event.button.y;
         break;

      case SDL_USEREVENT:
         event.type = Event::TIMER;
         break;

      default:
         break;
      }
   }

   return event.type;
}


namespace Event {

Type poll(Message& event)
{
   return getEvent(event, false);
}

Type wait(Message& event)
{
#ifdef PROJ_TARGET_Emscripten
   return getEvent(event, false);
#else
   return getEvent(event, true);
#endif
}

int mainLoop(bool (*callback_)(void*), void* user_data_)
{
   main_callback = callback_;
   user_data     = user_data_;

#ifdef PROJ_TARGET_Emscripten
   emscripten_set_main_loop(wrapLoopIter, 0, 1);
#else
   if(main_callback == nullptr)
   {
      printf("Press ENTER to quit\n");
      (void)getchar();
   }
   else
   {
      while(loopIter()) {}
   }
#endif

   return 0;
}

int eventLoop(void (*callback_)(const Message&, void*), void* user_data_)
{
   event_callback = callback_;

   return mainLoop(eventLoopIter, user_data_);
}


void setTimer(unsigned period_ms)
{
   static SDL_TimerID id = 0;

   if(id != 0)
   {
      SDL_RemoveTimer(id);
   }

   if(period_ms != 0)
   {
      id = SDL_AddTimer(period_ms, timer_callback, 0);
   }
}

} // Event

} // PLT
