//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Event implementation for micro::bit V2

#include "PLT/Event.h"

#include "MTL/Keyboard.h"

namespace PLT {

namespace Event {

static MTL::Keyboard<MTL::PIN_PAD_12,MTL::PIN_PAD_11> keyboard;
KBD_ATTACH_IRQ(keyboard)

Type poll(Message& event)
{
   uint8_t  ch;
   bool     up;

   if (keyboard.recv(ch, up))
   {
      event.type = up ? PLT::Event::KEY_UP : PLT::Event::KEY_DOWN;
      event.code = ch;
   }
   else
   {
      event.type = PLT::Event::NONE;
   }

   return event.type;
}

Type wait(Message& event)
{
   while(poll(event) == PLT::Event::NONE);

   return event.type;
}

int mainLoop(bool (*callback)(void*), void* user_data)
{
   if (callback == nullptr)
   {
      while(true);
   }

   while((*callback)(user_data));

   return 0;
}

int eventLoop(void (*callback)(const Message&, void*), void* user_data)
{
   while(true)
   {
      PLT::Event::Message event;
      PLT::Event::Type    type = PLT::Event::wait(event);

      if(type != PLT::Event::NONE)
      {
         if(callback != nullptr) (*callback)(event, user_data);
      }

      if (type == PLT::Event::QUIT) return 0;
   }
}

void setTimer(unsigned period_ms)
{
}

} // namespace Event

} // namespace PLT
