//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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
// \brief RP2350 SPI peripheral (an instance of Arm PL022)

#pragma once

#include "MTL/Periph.h"

#include "PadsBank.h"
#include "Pll.h"
#include "IoBank.h"
#include "Pins.h"

namespace MTL {

struct SpiReg
{
   uint32_t cr0;
   uint32_t cr1;
   uint32_t dr;
   uint32_t psr;
   uint32_t cpsr;
   uint32_t imsc;
   uint32_t ris;
   uint32_t mis;
   uint32_t icr;
   uint32_t macr;
};

//! Seiral Peripheral Interface
template <uint32_t BASE_ADDRESS,
         unsigned  CS_PIN,
         unsigned  CLK_PIN,
         unsigned  MOSI_PIN,
         unsigned  MISO_PIN>
class Spi
   : public Periph<SpiReg, BASE_ADDRESS>
{
public:
   Spi(unsigned freq, bool use_cs = true, bool use_miso = true)
   {
      PadsBank pads_bank;
      IoBank   io_bank;

      // Configure CLK pin
      pads_bank.setOut(CLK_PIN, PadsBank::DRIVE_2MA);
      io_bank.setFunc(CLK_PIN,  IoBank::SPI);

      // Configure MOSI pin
      pads_bank.setOut(MOSI_PIN, PadsBank::DRIVE_2MA);
      io_bank.setFunc(MOSI_PIN,  IoBank::SPI);

      // Configure CS pin if required
      if (use_cs)
      {
         pads_bank.setOut(CS_PIN, PadsBank::DRIVE_2MA);
         io_bank.setFunc(CS_PIN, IoBank::SPI);
      }

      // Configure MISO pin if required
      if (use_miso)
      {
         pads_bank.setOut(MISO_PIN, PadsBank::DRIVE_2MA);
         io_bank.setFunc(MISO_PIN,  IoBank::SPI);
      }

      unsigned divisor = PllSys().getFreq() / freq;
      unsigned cpsdvsr = 2;
      unsigned scr     = (divisor / cpsdvsr) - 1;

      this->reg->cpsr  = cpsdvsr;

      this->reg->cr0 = (/* clock_rate */       scr << 8) |
                       (/* SPH = 1st clock */    0 << 7) |
                       (/* SPO = clock low */    0 << 6) |
                       (/* FRF = Motorola */     0 << 4) |
                       (/* DSS = 8 bits   */ 0b111 << 0);

      this->reg->cr1 = (/* SOD = NA      */ 0 << 3) |
                       (/* MS  = master  */ 0 << 2) |
                       (/* SSE = enabled */ 1 << 1) |
                       (/* LBM = normal  */ 0 << 0);
    }

    void tx(uint8_t byte)
    {
       while(isTxFifoFull());

       this->reg->dr = byte;
    }

private:
   bool isBusy()        const { return this->getBit(this->reg->psr, 4) != 0; }
   bool isRxFifoFull()  const { return this->getBit(this->reg->psr, 3) != 0; }
   bool isRxFifoEmpty() const { return this->getBit(this->reg->psr, 2) == 0; }
   bool isTxFifoFull()  const { return this->getBit(this->reg->psr, 1) == 0; }
   bool isTxFifoEmpty() const { return this->getBit(this->reg->psr, 0) != 0; }
};

using Spi0      = Spi<0x40080000, IO_PIN_1,  IO_PIN_2,  IO_PIN_3,  IO_PIN_0>;
using Spi0_ALT1 = Spi<0x40080000, IO_PIN_5,  IO_PIN_6,  IO_PIN_7,  IO_PIN_4>;
using Spi0_ALT2 = Spi<0x40080000, IO_PIN_17, IO_PIN_18, IO_PIN_19, IO_PIN_16>;
using Spi0_ALT3 = Spi<0x40080000, IO_PIN_21, IO_PIN_22, IO_PIN_23, IO_PIN_20>;

using Spi1      = Spi<0x40088000, IO_PIN_9,  IO_PIN_10, IO_PIN_11, IO_PIN_8>;
using Spi1_ALT1 = Spi<0x40088000, IO_PIN_13, IO_PIN_14, IO_PIN_15, IO_PIN_12>;
using Spi1_ALT2 = Spi<0x40088000, IO_PIN_25, IO_PIN_26, IO_PIN_27, IO_PIN_24>;

} // namespace MTL
