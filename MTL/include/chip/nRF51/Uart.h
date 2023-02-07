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

#ifndef MTL_NRF51_UART_H
#define MTL_NRF51_UART_H

#include "MTL/Periph.h"
#include "MTL/nRF51/Gpio.h"

namespace MTL {

namespace nRF51 {

union UartReg
{
   // Tasks
   REG(0x000, startrx);
   REG(0x004, stoprx);
   REG(0x008, starttx);
   REG(0x00C, stoptx);
   REG(0x01C, suspend);

   // Events
   REG(0x100, cts);
   REG(0x104, ncts);
   REG(0x108, rxrdy);
   REG(0x11C, txrdy);
   REG(0x124, error);
   REG(0x144, rxto);

   // Registers
   REG(0x300, inten);
   REG(0x304, intenset);
   REG(0x308, intenclr);
   REG(0x480, errorsrc);
   REG(0x500, enable);
   REG(0x508, pselrts);
   REG(0x50C, pseltxd);
   REG(0x510, pselcts);
   REG(0x514, pselrxd);
   REG(0x518, rxd);
   REG(0x51C, txd);
   REG(0x524, baudrate);
   REG(0x56c, config);
};


template <unsigned PSELRXD,
          unsigned PSELTXD=0xFFFFFFFF,   // Default disconnected
          unsigned PSELCTS=0xFFFFFFFF,   // Default disconnected
          unsigned PSELRTS=0xFFFFFFFF>   // Default disconnected
class Uart : public Periph<UartReg,0x40002000>
{
private:
   MTL::Gpio::In<1,PSELRXD>  rxd;
   MTL::Gpio::Out<1,PSELTXD> txd;
   MTL::Gpio::In<1,PSELCTS>  cts;
   MTL::Gpio::Out<1,PSELRTS> rts;

public:
   // TODO calc baudrate constant from an actual frequency
   //      formulae is baud-rate * (1<<32) / 16000000
   //      round to nearest 0x1000
   Uart(uint32_t baud = 0x275000, bool parity = false)  // 9600 no-parity
   {
      reg->pselrxd  = PSELRXD;
      reg->pseltxd  = PSELTXD;
      reg->pselcts  = PSELCTS;
      reg->pselrts  = PSELRTS;

      reg->baudrate = baud;

      bool hwctl = (PSELCTS != 0xFFFFFFFF) && (PSELRTS != 0xFFFFFFFF);

      reg->config   = ((hwctl  ? 1 : 0)<<0) |
                      ((parity ? 7 : 0)<<1);

      reg->enable   = 0x4;

      if (PSELRXD != 0xFFFFFFFF)
      {
         reg->startrx = 1;
      }

      if (PSELTXD != 0xFFFFFFFF)
      {
         reg->starttx = 1;
      }
   }

   bool isRxFifoEmpty() const
   {
      return reg->rxrdy == 0;
   }

   bool isTxFifoEmpty() const
   {
      return reg->txrdy == 0;
   }

   uint8_t recv() const
   {
      reg->rxrdy = 0;
      return reg->rxd;
   }

   void send(uint8_t data)
   {
      reg->txrdy = 0;
      reg->txd = data;
   }

   uint8_t getError() const
   {
      uint8_t src = reg->errorsrc;
      reg->errorsrc = 0xF;
      return src;
   }
};

} // namespace nRF51

} // namespace MTL

#endif
