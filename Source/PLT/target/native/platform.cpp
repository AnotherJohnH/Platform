//------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
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

#include <atomic>
#include <cstdint>
#include <thread>

#include "MTL/Metal.h"
#include "GUI/Frame.h"
#include "PLT/Event.h"

std::atomic<uint8_t> gpio{0};

class VirtualDevice
{
public:
   VirtualDevice()
   {
      frame.clear(STB::BLACK);
      frame.refresh();

      PLT::Event::setTimer(50);
   }

   int eventLoop()
   {
      return PLT::Event::eventLoop(callback, this);
   }

private:
   static void callback(const PLT::Event::Message& event, void* ptr)
   {
      VirtualDevice* that = (VirtualDevice*)ptr;

      switch(event.type)
      {
      case PLT::Event::TIMER:
         that->redraw();
         break;

      default:
         break;
      }
   }

   void redraw()
   {
      for(size_t i=0; i<8; i++)
      {
         bool state = ((gpio >> i) & 1) != 0;
         if (state)
         {
            frame.fillRect(STB::RED,   10 + i * 40, 10, 30 + i * 40, 25);
         }
         else
         {
            frame.fillRect(STB::GREEN, 10 + i * 40, 10, 30 + i * 40, 25);
         }
      }

      frame.refresh();
   }

   GUI::Frame frame{"Metal", 400, 100};
};

int main()
{
   VirtualDevice device;

   std::thread thread{mtlMain};
   thread.detach();

   return device.eventLoop();
}
