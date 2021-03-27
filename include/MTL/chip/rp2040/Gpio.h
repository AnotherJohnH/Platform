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

// \file  Gpio.h
// \brief RP2040 General Purpose I/O

#ifndef MTL_RP2040_GPIO_H
#define MTL_RP2040_GPIO_H

#include "MTL/Periph.h"

namespace MTL {

namespace Gpio {

struct Reg
{
   struct
   {
      uint32_t status;
      uint32_t ctrl;

   } io[30];

   uint32_t intr[4];
   uint32_t proc0_irq_ctrl;
   uint32_t proc1_irq_ctrl;
   uint32_t dormant_wake_irq_ctrl;
};

static const uint32_t IRQOVER_NO_INVERT  = 0x0<<28;
static const uint32_t IRQOVER_INVERT     = 0x1<<28;
static const uint32_t IRQOVER_DRIVE_LOW  = 0x2<<28;
static const uint32_t IRQOVER_DRIVE_HIGH = 0x3<<28;

static const uint32_t INOVER_NO_INVERT   = 0x0<<16;
static const uint32_t INOVER_INVERT      = 0x1<<16;
static const uint32_t INOVER_DRIVE_LOW   = 0x2<<16;
static const uint32_t INOVER_DRIVE_HIGH  = 0x3<<16;

static const uint32_t OEOVER_PERIPH      = 0x0<<12;
static const uint32_t OEOVER_INV_PERIPH  = 0x1<<12;
static const uint32_t OEOVER_DISABLE     = 0x2<<12;
static const uint32_t OEOVER_ENABLE      = 0x3<<12;

static const uint32_t OUTOVER_PERIPH     = 0x0<<8;
static const uint32_t OUTOVER_INV_PERIPH = 0x1<<8;
static const uint32_t OUTOVER_DRIVE_LOW  = 0x2<<8;
static const uint32_t OUTOVER_DRIVE_HIGH = 0x3<<8;

static const uint32_t FUNCSEL            = 0;


template <unsigned WIDTH, unsigned PIN>
class Out : public Periph<Reg, 0x40014000>
{
public:
   Out()
   {
      reg->io[PIN].ctrl = OEOVER_ENABLE | FUNCSEL;
   }

   operator uint32_t() const
   {
      return 0;
   }

   uint32_t operator=(uint32_t data)
   {
      return data;
   }

   void set(uint32_t data)
   {
      reg->io[PIN].ctrl = (reg->io[PIN].ctrl & 0xFFFFF0FF) | OUTOVER_DRIVE_HIGH;
   }

   void clr(uint32_t data)
   {
      reg->io[PIN].ctrl = (reg->io[PIN].ctrl & 0xFFFFF0FF) | OUTOVER_DRIVE_LOW;
   }

private:
   static const unsigned LSB       = PIN & 0x1F;
   static const unsigned MSB       = LSB + WIDTH - 1;
   static const uint32_t DATA_MASK = (1<<WIDTH) - 1;
};

template <unsigned WIDTH, unsigned PIN>
class In : public Periph<Reg, 0x50000000>
{
public:
   In()
   {
   }

   operator uint32_t() const
   {
      return 0;
   }

private:
   static const unsigned LSB       = PIN & 0x1F;
   static const unsigned MSB       = LSB + WIDTH - 1;
   static const uint32_t DATA_MASK = (1<<WIDTH) - 1;
};

} // namespace Gpio

} // namespace MTL

#endif // MTL_RP2040_GPIO_H
