//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MTL/Periph.h"
#include "MTL/nRF51/Gpio.h"

namespace MTL {

namespace nRF51 {

constexpr uint32_t UART_BAUD_9600   = 0x00275000;
constexpr uint32_t UART_BAUD_115200 = 0x01D7E000;

union UartReg
{
   // Tasks
   REG(0x000, task_startrx);
   REG(0x004, task_stoprx);
   REG(0x008, task_starttx);
   REG(0x00C, task_stoptx);
   REG(0x01C, task_suspend);

   // Events
   REG(0x100, event_cts);
   REG(0x104, event_ncts);
   REG(0x108, event_rxrdy);
   REG(0x11C, event_txrdy);
   REG(0x124, event_error);
   REG(0x144, event_rxto);

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

template <unsigned PSEL_RXD,
          unsigned PSEL_TXD=0xFFFFFFFF, // Default disconnected
          unsigned PSEL_CTS=0xFFFFFFFF, // Default disconnected
          unsigned PSEL_RTS=0xFFFFFFFF> // Default disconnected
class Uart : public Periph<UartReg,0x40002000>
{
public:
   // TODO calc baudrate constant from an actual frequency
   //      formulae is baud-rate * (1<<32) / 16000000
   //      round to nearest 0x1000
   Uart(uint32_t baud = UART_BAUD_9600, bool parity = false)  // 9600 no-parity
   {
      reg->pselrxd = PSEL_RXD;
      reg->pseltxd = PSEL_TXD;
      reg->pselcts = PSEL_CTS;
      reg->pselrts = PSEL_RTS;

      reg->baudrate = baud;

      bool hwctl = (PSEL_CTS != 0xFFFFFFFF) || (PSEL_RTS != 0xFFFFFFFF);

      reg->config = ((parity ? 0b111 : 0b000) << 1) | ((hwctl ? 1 : 0) << 0);
      reg->enable = 4;

      if (PSEL_RXD != 0xFFFFFFFF)
      {
         reg->task_startrx = 1;
      }

      if (PSEL_TXD != 0xFFFFFFFF)
      {
         reg->task_starttx = 1;
      }

      // Seem to need a delay here
      for(volatile unsigned i = 0; i < 1000; ++i);
   }

   bool isRxFifoEmpty() const
   {
      return reg->event_rxrdy == 0;
   }

   bool isTxFifoEmpty() const
   {
      return reg->event_txrdy == 0;
   }

   void tx(uint8_t data)
   {
      reg->event_txrdy = 0;
      reg->txd         = data;

      while(reg->event_txrdy == 0);
   }

   uint8_t rx() const
   {
      reg->event_rxrdy = 0;
      return reg->rxd;
   }

   uint8_t getError() const
   {
      uint8_t src = reg->errorsrc;
      reg->errorsrc = 0xF;
      return src;
   }

private:
   MTL::Gpio::In<1,PSEL_RXD>  rxd;
   MTL::Gpio::Out<1,PSEL_TXD> txd;
   MTL::Gpio::In<1,PSEL_CTS>  cts;
   MTL::Gpio::Out<1,PSEL_RTS> rts;
};

} // namespace nRF51

} // namespace MTL

