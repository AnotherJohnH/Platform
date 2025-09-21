//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Stub Event implementation

#include "PLT/Event.h"

namespace PLT {

namespace Event {

Type poll(Message& event)
{
   return PLT::Event::NONE;
}

Type wait(Message& event)
{
   return PLT::Event::NONE;
}

int mainLoop(bool (*callback)(void*), void* user_data)
{
   return 0;
}

int eventLoop(void (*callback)(const Message&, void*), void* user_data)
{
   return 0;
}

void setTimer(unsigned period_ms)
{
}

} // namespace Event

} // namespace PLT
