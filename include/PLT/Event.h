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


//! Identify type of a user interface event
enum EventType : uint8_t
{
   NONE = 0,     //!< No event
   KEY_DOWN,     //!< A key has been pressed
   KEY_UP,       //!< A key has been release
   BUTTON_DOWN,  //!< Screen touch or mouse button pressed
   BUTTON_UP,    //!< Screen untouched or mouse button released
   POINTER_MOVE, //!< Swipe or mouse move
   TIMER,        //!< Repeating timer event requested by setTimer()
   RESIZE,       //!< Windows resized
   QUIT          //!< Application quit
};


//! Mouse button
enum class Button : uint8_t
{
   NONE = 0,
   LEFT,
   RIGHT
};


//! A record of an event
struct Event
{
   EventType type{NONE};   //!< Event type
   uint8_t   code{0};      //!< Identify key or mouse button
   uint16_t  x{0};         //!< Mouse pointer or touch x co-ordinate
   uint16_t  y{0};         //!< Mouse pointer or touch y-co-ordinate
};


//! Poll for the next user interface event from the event queue
//
//! This method will not block if there are no events
//! \param event Event record for next event
//! \return the type of event
EventType pollEvent(Event& event);

//! Wait for the next user interface event from the event queue
//
//! This method will block until an event arrives.
//! \param event event
//! \return the type of event
EventType waitEvent(Event& event);

//! Enter an infinite loop until the application quits
int mainLoop(bool (*callback)(void*) = nullptr, void* user_ptr = nullptr);

//! Enter an event loop until the application quits
int eventLoop(void (*callback)(const Event&, void*) = nullptr, void* user_ptr = nullptr);

//! Request repeating timer events
void setTimer(unsigned period_ms);


} // namespace PLT

#endif
