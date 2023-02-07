//------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
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
//
// NORDIC Semiconductor
// nRF52
// GPIO-TE peripheral
//

#ifndef MTL_NRF52_GPIO_TE_H
#define MTL_NRF52_GPIO_TE_H

#include "MTL/Periph.h"
#include "MTL/nRF52/Irq.h"
#include "MTL/CortexM4/NVIC.h"

namespace MTL {

namespace nRF52 {

union GpioTEReg
{
   REG_ARRAY(0x000, tasks_out,  8);
   REG_ARRAY(0x030, tasks_set,  8);
   REG_ARRAY(0x060, tasks_clr,  8);
   REG_ARRAY(0x100, events_in,  8);
   REG(      0x17C, events_port);
   REG(      0x304, intenset);
   REG(      0x308, intenclr);
   REG_ARRAY(0x510, config,     8);
};

class GpioTE : public Periph<GpioTEReg,0x40006000>
{
public:
   enum Event { NONE = 0, LO_TO_HI = 1, HI_TO_LO = 2, TOGGLE = 3 };

   GpioTE() = default;

   //! Allocate an output pin for GPIO-TE
   unsigned allocTask(unsigned pin, Event event = TOGGLE)
   {
      unsigned index = getNextFree();
      if (index == 8) return BAD_INDEX;

      uint32_t rw_mode  = 0b11; // Task mode
      uint32_t psel     = pin & 0b11111;
      uint32_t port     = (pin >> 8) & 0b1;
      uint32_t polarity = 0b11; // Toggle
      uint32_t outinit  = 0b0;

      reg->config[index] = (outinit  << 20) |
                           (polarity << 16) |
                           (port     << 13) |
                           (psel     <<  8) |
                           (rw_mode  <<  0);

      return index;
   }

   //! Allocate an input pin for GPIO-TE
   unsigned allocEvent(unsigned pin, Event event = TOGGLE)
   {
      unsigned index = getNextFree();
      if (index == 8) return BAD_INDEX;

      uint32_t rw_mode  = 0b01; // Event mode
      uint32_t psel     = pin & 0b11111;
      uint32_t port     = (pin >> 8) & 0b1;
      uint32_t polarity = event;
      uint32_t outinit  = 0b0;

      reg->config[index] = (outinit  << 20) |
                           (polarity << 16) |
                           (port     << 13) |
                           (psel     <<  8) |
                           (rw_mode  <<  0);

      return index;
   }

   uint32_t getTaskSet(unsigned index) const { return uint32_t(&reg->tasks_set[index]); }
   uint32_t getTaskClr(unsigned index) const { return uint32_t(&reg->tasks_clr[index]); }
   uint32_t getTaskTgl(unsigned index) const { return uint32_t(&reg->tasks_out[index]); }

   void clrEvent(unsigned index)
   {
      reg->events_in[index] = 0;
   }

   void enableIrq(unsigned index)
   {
      irq.enable();

      reg->intenset = 1<<index;
   }

   void disableIrq(unsigned index)
   {
      reg->intenclr = 1<<index;
   }

   static const unsigned BAD_INDEX{0xFFFFffff};

private:
   using Periph<GpioTEReg,0x40006000>::reg;

   unsigned getNextFree()
   {
      static unsigned next_free{0};
      return next_free++;
   }

   NVIC<IRQ_GPIO_TE> irq;
};

} // namespace nRF52

} // namespace MTL

#endif
