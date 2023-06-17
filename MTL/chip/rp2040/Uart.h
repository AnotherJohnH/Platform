//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

// \brief RP2040 UART peripheral

#pragma once

#include "MTL/Periph.h"

#include "PadsBank.h"
#include "Pll.h"
#include "IoBank.h"
#include "Pins.h"

namespace MTL {

namespace UART { enum Parity { NONE, ODD, EVEN }; }

struct UartReg
{
   uint32_t dr;
   uint32_t rsr;
   uint32_t pad1[4];
   uint32_t tfr;
   uint32_t pad2;
   uint32_t ilpr;
   uint32_t ibrd;
   uint32_t fbrd;
   uint32_t lcr_h;
   uint32_t cr;
   uint32_t ifls;
   uint32_t imsc;
   uint32_t ris;
   uint32_t mis;
   uint32_t icr;
   uint32_t dmacr;
};

template <uint32_t BASE_ADDRESS, unsigned TX_PIN, unsigned RX_PIN>
class Uart : public Periph<UartReg, BASE_ADDRESS>
{
public:
   Uart(unsigned     baud,
        unsigned     data_bits = 8,
        UART::Parity parity    = UART::NONE,
        unsigned     stop_bits = 1)
   {
      // Configure TX pin
      pads_bank.setOut(TX_PIN, PadsBank::DRIVE_2MA);
      io_bank.setFunc(TX_PIN, IoBank::UART);

      // Configure RX pin
      pads_bank.setIn(RX_PIN);
      io_bank.setFunc(RX_PIN, IoBank::UART);

      // Setup baud rate
      uint32_t divisor = (8 * PllSys().getFreq()) / baud;
      this->reg->ibrd = divisor >> 7;
      this->reg->fbrd = ((divisor & 0x7F) + 1) >> 1;

      // Setup format
      uint32_t lcr_h = 0;
      this->setField(lcr_h, 6, 5, data_bits - 5);        // WLEN
      this->setBit(  lcr_h, 4,    1);                    // FEN
      this->setBit(  lcr_h, 3,    stop_bits - 1);        // STP2
      this->setBit(  lcr_h, 2,    parity == UART::EVEN); // EPS
      this->setBit(  lcr_h, 1,    parity != UART::NONE); // PEN
      this->reg->lcr_h = lcr_h;

      // Enable
      this->reg->cr = (1 << 9) |  // TXEN
                      (1 << 8) |  // RXEN
                      (1 << 0);   // EN
   }

   //! Check if RX FIFO is empty
   bool empty() const
   {
      return this->getBit(this->reg->tfr, 4) != 0;
   }

   //! Check if TX FIFO is full
   bool full() const
   {
      return this->getBit(this->reg->tfr, 5) != 0;
   }

   void tx(uint8_t data)
   {
      // Block when FIFO full
      while(full());

      this->reg->dr = data;
   }

   uint8_t rx()
   {
      // Block when FIFO empty
      while(empty());

      return this->reg->dr;
   }

private:
   PadsBank pads_bank;
   IoBank   io_bank;
};

using Uart0      = Uart<0x40034000, MTL::rp2040::IO_PIN_0,  MTL::rp2040::IO_PIN_1>;
using Uart0_ALT1 = Uart<0x40034000, MTL::rp2040::IO_PIN_12, MTL::rp2040::IO_PIN_13>;
using Uart0_ALT2 = Uart<0x40034000, MTL::rp2040::IO_PIN_16, MTL::rp2040::IO_PIN_17>;

using Uart1      = Uart<0x40038000, MTL::rp2040::IO_PIN_4, MTL::rp2040::IO_PIN_5>;
using Uart1_ALT  = Uart<0x40038000, MTL::rp2040::IO_PIN_8, MTL::rp2040::IO_PIN_9>;

} // namespace MTL
