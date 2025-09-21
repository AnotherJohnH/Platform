//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MTL/MTL.h"
#include "MTL/Pins.h"
#include "MTL/nRF51/Gpio.h"
#include "MTL/nRF51/Timer.h"

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
