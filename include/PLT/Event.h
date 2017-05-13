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

#ifndef PLT_EVENT_H
#define PLT_EVENT_H

#include <cstdint>

#include "PLT/KeyCode.h"

namespace PLT {


enum EventType : uint8_t
{
   NONE,
   KEY_DOWN,
   KEY_UP,
   BUTTON_DOWN,
   BUTTON_UP,
   POINTER_MOVE,
   TIMER,
   RESIZE,
   QUIT
};


enum class Button : uint8_t
{
   NONE,
   LEFT,
   RIGHT
};


class Event
{
public:
   EventType   type = NONE;
   uint8_t     code{};
   uint16_t    x{};
   uint16_t    y{};
};


//! Get next user interface event from the queue
//  \param event event
//  \return the type of event
EventType pollEvent(Event& event);

//! Wait for the next user interface event from the queue
//  \param event event
//  \return the type of event
EventType waitEvent(Event& event);


void setTimer(unsigned period_ms);


} // namespace PLT

#endif
