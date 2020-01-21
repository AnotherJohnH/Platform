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

#ifndef MICROBIT_LEDS_H
#define MICROBIT_LEDS_H

#include "MTL/Metal.h"
#include "MTL/Gpio.h"
#include "MTL/chip/nrF51/Timer.h"
#include "MTL/Pins.h"

#define  SCAN_TIMER  2


#define LEDS_ATTACH_IRQ(LEDS) \
     extern "C" { void Timer_2_IRQ() { LEDS.scan(); } }

namespace MTL {

class Leds
{
private:
   static const unsigned NUM_COLS  = 9;
   static const unsigned NUM_ROWS  = 3;
   static const unsigned NUM_BITS  = NUM_COLS + NUM_ROWS;

   static const uint8_t map[5][5];

   uint16_t                         frame[NUM_ROWS];
   uint8_t                          row;
   bool                             irq;
   Gpio::Out<NUM_BITS,PIN_LED_COL1> gpio;
   nRF51::Timer<SCAN_TIMER>         timer;

public:
   Leds(bool irq_ = true) : row(0), irq(irq_)
   {
      for(unsigned i=0; i<NUM_ROWS; ++i)
      {
         frame[i] = (1<<(NUM_COLS + i)) | ((1<<NUM_COLS)-1);
         scan();
      }

      if (irq)
      {
         timer.setClock(nRF51::TIMER_250_KHZ);
         timer.setAction(0, 1000, nRF51::TIMER_CLEAR | nRF51::TIMER_IRQ);
         timer.start();
      }
   }

   //! Drive LEDs
   void scan()
   {
      gpio = frame[row];

      if (row++ == NUM_ROWS)
      {
         row = 0;
      }

      if (irq) timer.clrCompareEvent(0);
   }

   //! Set state of a single LED
   void write(unsigned x, unsigned y, bool state) volatile
   {
      unsigned row  = map[y][x] >> 4;
      unsigned col  = map[y][x] & 0xF;
      uint16_t mask = 1<<col;

      if (state)
         frame[row] &= ~mask;
      else
         frame[row] |= mask;
   }

   //! Set state of a line of LEDs
   void write(unsigned y, uint8_t data5)
   {
      for(unsigned x=0; x<5; ++x)
      {
         write(x, y, (data5 & (1<<(4-x))) != 0);
      }
   }

   //! Set state of all LEDs
   void write(uint8_t l0, uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4)
   {
      write(0, l0);
      write(1, l1);
      write(2, l2);
      write(3, l3);
      write(4, l4);
   }
};

} // namespace MTL

#endif
