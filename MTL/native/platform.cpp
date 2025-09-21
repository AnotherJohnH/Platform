//-------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <atomic>
#include <cstdint>
#include <thread>

#include "MTL/Metal.h"
#include "GUI/Frame.h"
#include "PLT/Event.h"

std::atomic<uint8_t> gpio{0};

//! Virtual Metal platform
class VirtualMetalPlatform
{
public:
   VirtualMetalPlatform()
   {
      frame.clear(STB::BLACK);
      frame.refresh();

      PLT::Event::setTimer(1000 / REFRESH_RATE_HZ);
   }

   int eventLoop()
   {
      return PLT::Event::eventLoop(callback, this);
   }

private:
   static void callback(const PLT::Event::Message& event, void* ptr)
   {
      VirtualMetalPlatform* that = (VirtualMetalPlatform*)ptr;
      that->handleEvent(event);
   }

   void handleEvent(const PLT::Event::Message& event)
   {
      switch(event.type)
      {
      case PLT::Event::TIMER:
         if (launch_application)
         {
             launch_application = false;

             // Start the metal application
             std::thread thread{MTL_main};
             thread.detach();
         }
         redraw();
         break;

      default:
         break;
      }
   }

   void drawLED(unsigned x, unsigned y, bool state)
   {
      STB::Colour colour = state ? STB::RED
                                 : STB::RGB(0x40, 0, 0);

      frame.fillRect(colour, x, y, x + 20, y + 15);
   }

   void redraw()
   {
      for(size_t i=0; i<8; i++)
      {
         unsigned x     = 10 + (8 - i) * 40;
         unsigned y     = 10;
         bool     state = ((gpio >> i) & 1) != 0;

         drawLED(x, y, state);
      }

      frame.refresh();
   }

   static const unsigned REFRESH_RATE_HZ = 20;

   GUI::Frame frame{"Metal", 400, 100};
   bool       launch_application{true};
};

int main()
{
   return VirtualMetalPlatform().eventLoop();
}
