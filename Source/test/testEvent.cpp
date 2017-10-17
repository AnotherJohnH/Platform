//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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

#include <cstdio>

#include "PLT/Event.h"
#include "PLT/Frame.h"

void callback(const PLT::Event& event, void* ptr)
{
   switch(event.type)
   {
   case PLT::KEY_DOWN:     printf("KEY_DOWN     %02X\n",     event.code); break;
   case PLT::KEY_UP:       printf("KEY_UP       %02X\n",     event.code); break;
   case PLT::BUTTON_DOWN:  printf("BUTTON_DOWN  %d %u %u\n", event.code, event.x, event.y); break;
   case PLT::BUTTON_UP:    printf("BUTTON_UP    %d %u %u\n", event.code, event.x, event.y); break;
   case PLT::POINTER_MOVE: printf("POINTER_MOVE %u %u\n",    event.x, event.y); break;
   case PLT::TIMER:        printf("TIMER\n"); break;
   case PLT::RESIZE:       printf("RESIZE\n"); break;
   case PLT::QUIT:         printf("QUIT\n"); break;

   default: break;
   }
}

int main(int argc, const char* argv[])
{
   PLT::Frame frame("Event Test", 100, 100);

   PLT::setTimer(1000);

   return PLT::eventLoop(callback);
}
