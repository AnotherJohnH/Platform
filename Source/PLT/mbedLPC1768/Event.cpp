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

// \brief Event implementation for mbed LPC1768

#include "PLT/Event.h"

#include "../../MTL/chip/LPC1768/Keyboard.h"

namespace PLT {

namespace Event {


static MTL::Keyboard  keyboard;


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
