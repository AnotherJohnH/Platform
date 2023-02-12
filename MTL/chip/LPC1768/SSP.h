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

// \brief NXP LPC1768 Synchronous Serial Port
//
// Data source NXP document "LPC17xx User Manual UM10360"

#pragma once

#include "MTL/Periph.h"

#include "SysCon.h"
#include "PinCon.h"

namespace MTL {

union SSPReg
{
   REG(0x000, cr0);   //!< Control 0
   REG(0x004, cr1);   //!< Control 1
   REG(0x008, dr);    //!< Data
   REG(0x00C, sr);    //!< Status
   REG(0x010, cpsr);  //!< Clock prescale
   REG(0x014, imsc);  //!< Interrupt mask set and clear
   REG(0x018, ris);   //!< Raw interrupt status
   REG(0x01C, mis);   //!< Masked interrupt status
   REG(0x020, icr);   //!< SSPICR interrupt clear
   REG(0x024, dmacr); //!< DMA control register
};

template <unsigned INDEX>
class SSP : public Periph<SSPReg,INDEX == 0 ? 0x40088000
                                            : 0x40030000>
{
private:
   // Frame format
   static const unsigned CR0_FRF_SPI       = (0<<4);
   static const unsigned CR0_FRF_TI        = (1<<4);
   static const unsigned CR0_FRF_MICROWIRE = (2<<4);

   // Clock out polarity
   static const unsigned CR0_CPOL_LO  = (0<<6);
   static const unsigned CR0_CPOL_HI  = (1<<6);

   // Clock out phase
   static const unsigned CR0_CPHA_FIRST = (0<<7);
   static const unsigned CR0_CPHA_SECND = (1<<7);

public:
   SSP(unsigned bits, bool master)
   {
      // Power up and selct PCLK/1 clock
      if (INDEX == 0)
      {
         SysCon().pconp(21, true);  // PCSSP0 (already enabled at reset)
         SysCon().pclksel1(10, 1);  // PCLK_SSP0
      }
      else if (INDEX == 1)
      {
         SysCon().pconp(10, true);  // PCSSP1 (already enabled at reset)
         SysCon().pclksel0(20, 1);  // PCLK_SSP1
      }

      configure(bits, master);
   }

   void configure(unsigned bits, bool master)
   {
      this->reg->cr0 = ((bits - 1) << 0)     // Frame length (bits)
                     | CR0_FRF_SPI
                     | CR0_CPOL_HI
                     | CR0_CPHA_SECND
                     | (0          << 8);    // Serial clock rate

      this->reg->cr1 = (0 << 0)              // LBM  Loop back
                     | (1 << 1)              // SSE  Enable
                     | ((master?0:1) << 2)   // MS   Master
                     | ((master?0:1) << 3);  // SOD  Slave out disable
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
