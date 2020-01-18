//------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
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

// \brief 

#include <cstdio>
#include <ctime>

#include "TRM/Frame.h"

#include "MTL/chip/LPC1768/SSP.h"
#include "MTL/chip/LPC1768/TIMER.h"
#include "MTL/core/CortexM3/SysTimer.h"

#include "MTL/Gpio.h"
#include "MTL/Pins.h"

namespace MTL {


class Mouse
{
private:
   SSP<0>    ssp;

public:
   Mouse() : ssp(11, /* master */ false)
   {
      ssp.enablePin_MOSI(); // DIP11
      ssp.enablePin_MISO(); // DIP12
      ssp.enablePin_SCK();  // DIP13
      ssp.enablePin_SSEL(); // DIP14
   }

   bool recv(uint8_t& byte)
   {
      while(!ssp.isRxFifoEmpty())
      {
         // Remove stop and parity bits
         byte = uint8_t(ssp.recv() >> 1);

         // Reverse remaining bits
         byte = ((byte & 0xAA) >> 1) | ((byte & 0x55) << 1);
         byte = ((byte & 0xCC) >> 2) | ((byte & 0x33) << 2);
         byte = ((byte & 0xF0) >> 4) | ((byte & 0x0F) << 4);
         return true;
      }

      return false;
   }

   void send(uint8_t byte)
   {
      uint32_t parity = byte;
      parity ^= parity >> 4;
      parity ^= parity >> 2;
      parity ^= parity >> 1;
      parity &= 1;

      uint32_t packet = (1<<10)     // STOP
                      | (parity<<9) 
                      | (byte<<1)
                      | (0<<0);     // START

      ssp.send(packet);

      // Connect GPIO to clock and data lines
      Gpio::Out<1,PIN_DIP13>  clk;
      Gpio::Out<1,PIN_DIP11>  data;

      // Request to TX
      clk = false;
      //delay(100);

      data = false;
      //delay(10);

      clk  = true;

      // Restore SSP pin connection
      ssp.enablePin_MOSI(); // DIP11
      ssp.enablePin_SCK();  // DIP13
   }
};

}

int main()
{
   TRM::Frame<512,512>  frame("Test mouse");
   MTL::Mouse           mouse;

   frame.write("Mouse test...\n");

   mouse.send(0xFF);

   while(true)
   {
      char buffer[20];
      uint8_t ch;
      
      if (mouse.recv(ch))
      {
         sprintf(buffer, "[%02X] ", ch);
         frame.write(buffer);

         if (ch == 0xFF) 
         {
            frame.write('\n');
         }
      }
   }
}
