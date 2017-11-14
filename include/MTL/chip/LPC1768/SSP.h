//------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
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

// \file SSP.h
// \brief NXP LPC1768 Synchronous Serial Port
//
// Data source NXP document "LPC17xx User Manual UM10360"

#ifndef LPC1768_SSP_H
#define LPC1768_SSP_H

#include "MTL/Periph.h"

#include "SysCon.h"
#include "PinCon.h"


union SSPReg
{
   REG(0x000, cr0);
   REG(0x004, cr1);
   REG(0x008, dr);
   REG(0x00C, sr);
   REG(0x010, cpsr);
   REG(0x014, imsc);
   REG(0x018, ris);
   REG(0x01C, mis);
   REG(0x020, icr);
   REG(0x024, dmacr);
};


template <unsigned INDEX>
class SSP : public Periph<SSPReg,INDEX == 0 ? 0x40088000
                                            : 0x40030000>
{
public:
   SSP(unsigned bits, bool master)
   {
      // Power up and selct PCLK/1 clock
      if (INDEX == 0)
      {
         SysCon().pconp(21, true);
         SysCon().pclksel1(10, 1);
      }
      else if (INDEX == 1)
      {
         SysCon().pconp(10, true);
         SysCon().pclksel1(20, 1);
      }

      // Configure

      this->reg->cr0 = ((bits - 1) << 0)     // Frame length (bits)
                     | (0          << 4)     // SPI
                     | (1          << 6)     // Clock out polarity
                     | (1          << 7)     // Clock out phase
                     | (0          << 8);    // Serial clock rate

      this->reg->cr1 = (0 << 0)              // Loop back
                     | (1 << 1)              // Enable
                     | ((master?0:1) << 2)   // Master
                     | (1 << 3);             // Slave out disable
   }

   void enablePin_SSEL()
   {
      if (INDEX == 0)
         // SSP0_SSEL (mbed DIP14)
         PinCon().config(PIN_0_16, 2, PinCon::PULL_NONE);
      else if (INDEX == 1)
         // SSP1_SSEL (mbed DIP8)
         PinCon().config(PIN_0_6, 2, PinCon::PULL_NONE);
   }

   void enablePin_SCK()
   {
      if (INDEX == 0)
         // SSP0_SCK (mbed DIP13)
         PinCon().config(PIN_0_15, 2, PinCon::PULL_NONE);
      else if (INDEX == 1)
         // SSP1_SCK (mbed DIP7)
         PinCon().config(PIN_0_7, 2, PinCon::PULL_NONE);
   }

   void enablePin_MISO()
   {
      if (INDEX == 0)
         // SSP0_MISO   (mbed DIP12)
         PinCon().config(PIN_0_17, 2, PinCon::PULL_NONE);
      else if (INDEX == 1)
         // SSP1_MISO   (mbed DIP6)
         PinCon().config(PIN_0_8, 2, PinCon::PULL_NONE);

      this->reg->cr1.clrBit(3);
   }

   void enablePin_MOSI()
   {
      if (INDEX == 0)
         // SSP0_MOSI   (mbed DIP11)
         PinCon().config(PIN_0_18, 2, PinCon::PULL_NONE);
      else if (INDEX == 1)
         // SSP1_MOSI   (mbed DIP5)
         PinCon().config(PIN_0_9, 2, PinCon::PULL_NONE);
   }

   bool isRxFifoEmpty() const
   {
      return !this->reg->sr.getBit(2);
   }

   //! Write data to TX FIFO
   void send(uint32_t data)
   {
      this->reg->dr = data;
   }

   //! Read data from RX FIFO
   uint32_t recv()
   {
      return this->reg->dr;
   }
};


} // namespace MTL

#endif // LPC1768_SSP_H
