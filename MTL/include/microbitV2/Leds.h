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

#ifndef MTL_MICROBIT_V2_LEDS_H
#define MTL_MICROBIT_V2_LEDS_H

#include "MTL/Metal.h"
#include "MTL/Pins.h"
#include "MTL/nRF52/Gpio.h"
#include "MTL/nRF52/Timer.h"

#define LEDS_ATTACH_IRQ(LEDS) \
     extern "C" { void Timer_2_IRQ() { LEDS.scan(); } }

namespace MTL {

class Leds
{
private:
   static const unsigned NUM_COLS = 5;
   static const unsigned NUM_ROWS = 5;

   volatile uint8_t          frame[NUM_COLS];
   volatile uint8_t          col{0};
   Gpio::Out<1,PIN_LED_COL1> col1_out;
   Gpio::Out<1,PIN_LED_COL2> col2_out;
   Gpio::Out<1,PIN_LED_COL3> col3_out;
   Gpio::Out<1,PIN_LED_COL4> col4_out;
   Gpio::Out<1,PIN_LED_COL5> col5_out;
   Gpio::Out<1,PIN_LED_ROW1> row1_out;
   Gpio::Out<1,PIN_LED_ROW2> row2_out;
   Gpio::Out<1,PIN_LED_ROW3> row3_out;
   Gpio::Out<1,PIN_LED_ROW4> row4_out;
   Gpio::Out<1,PIN_LED_ROW5> row5_out;
   nRF52::Timer2             timer;

   void setCol(unsigned index, bool state)
   {
      switch(index)
      {
      case 0: col1_out = state; break;
      case 1: col2_out = state; break;
      case 2: col3_out = state; break;
      case 3: col4_out = state; break;
      case 4: col5_out = state; break;
      }
   }

   void setRow(unsigned index, bool state)
   {
      switch(index)
      {
      case 0: row1_out = state; break;
      case 1: row2_out = state; break;
      case 2: row3_out = state; break;
      case 3: row4_out = state; break;
      case 4: row5_out = state; break;
      }
   }

public:
   Leds()
   {
      timer.setClock(nRF52::TIMER_250_KHZ);
      timer.setAction(0, 1000, nRF52::TIMER_CLEAR | nRF52::TIMER_IRQ);
      timer.start();
   }

   //! Drive LEDs
   void scan()
   {
      timer.clrCompareEvent(0);

      setCol(col, true);

      if (++col == NUM_COLS)
      {
         col = 0;
      }

      for(unsigned row = 0; row < NUM_ROWS; ++row)
      {
         setRow(row, (frame[col] & (1<<row)) != 0);
      }

      setCol(col, false);
   }

   //! Set state of a single LED
   void write(unsigned x, unsigned y, bool state) volatile
   {
      uint8_t mask = 1<<y;

      if (state)
         frame[x] |= mask;
      else
         frame[x] &= ~mask;
   }

   //! Set state of a line of LEDs
   void write(unsigned y, uint8_t data5)
   {
      for(unsigned x=0; x<NUM_COLS; ++x)
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
