//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MTL/Periph.h"
#include "MTL/nRF52/Pins.h"
#include "MTL/nRF52/Gpio.h"

namespace MTL {

namespace nRF52 {

union UartReg
{
   REG(0x000, tasks_startrx);
   REG(0x004, tasks_stoprx);
   REG(0x008, tasks_starttx);
   REG(0x00C, tasks_stoptx);
   REG(0x02C, tasks_flushrx);

   REG(0x100, events_cts);
   REG(0x104, events_ncts);
   REG(0x108, events_rxdrdy);
   REG(0x110, events_endrx);
   REG(0x11C, events_txdrdy);
   REG(0x120, events_endtx);
   REG(0x124, events_error);
   REG(0x144, events_rxto);
   REG(0x14C, events_rxstarted);
   REG(0x150, events_txstarted);
   REG(0x158, events_txstopped);

   REG(0x200, shorts);

   REG(0x300, inten);
   REG(0x304, intenset);
   REG(0x308, intenclr);
   REG(0x480, errorsrc);
   REG(0x500, enable);
   REG(0x508, psel_rts);
   REG(0x50C, psel_txd);
   REG(0x510, psel_cts);
   REG(0x514, psel_rxd);
   REG(0x524, baudrate);
   REG(0x534, rxd_ptr);
   REG(0x538, rxd_maxcnt);
   REG(0x53C, rxd_amount);
   REG(0x544, txd_ptr);
   REG(0x548, txd_maxcnt);
   REG(0x54C, txd_amount);
   REG(0x56C, config);
};

template <uint32_t BASE,
          unsigned PIN_RXD,
          unsigned PIN_TXD,
          unsigned PIN_CTS,
          unsigned PIN_RTS>
class UartInstance : public Periph<UartReg,BASE>
{
public:
   // TODO calc baudrate constant from an actual frequency
   //      formulae is baud-rate * (1<<32) / 16000000
   //      round to nearest 0x1000
   UartInstance(uint32_t baud = 0x275000, bool parity = false)  // 9600 no-parity
   {
      reg->psel_rxd = PIN_RXD;
      reg->psel_txd = PIN_TXD;
      reg->psel_cts = PIN_CTS;
      reg->psel_rts = PIN_RTS;

      reg->baudrate = baud;

      bool hwctl = (PIN_CTS != PIN_NULL) &&
                   (PIN_RTS != PIN_NULL);

      reg->config = ((hwctl  ? 1 : 0)<<0) |
                    ((parity ? 7 : 0)<<1);

      reg->enable = 8;

      if (PIN_RXD != PIN_NULL)
      {
         reg->shorts     = 1<<6; // ENDRX_STARTRX
         reg->rxd_ptr    = uint32_t(rx_buffer) ^ 0x20800000;
         reg->rxd_maxcnt = RX_BUFFER_SIZE;

         doStartRx();
      }
   }

   bool isRxFifoEmpty() const
   {
      if (reg->events_rxdrdy)
      {
         reg->events_rxdrdy = 0;
         reg->tasks_flushrx = 1;
      }
      if (reg->events_endrx)
      {
         doStartRx();
      }
      return reg->rxd_amount == rx_index;
   }

   uint8_t recv()
   {
      uint8_t data = rx_buffer[rx_index++];
      if (rx_index == RX_BUFFER_SIZE)
      {
         rx_index = 0;
      }
      return data;
   }

   bool isTxFifoEmpty() const
   {
      return reg->events_txrdy == 0;
   }

   void send(uint8_t data)
   {
      // XXX need some flow control here
      reg->events_txrdy  = 0;
      reg->txd_ptr       = uint32_t(&data) ^ 0x20800000;
      reg->txd_maxcnt    = 1;
      reg->tasks_starttx = 1;
   }

   uint8_t getError() const
   {
      uint8_t src = reg->errorsrc;
      reg->errorsrc = 0xF;
      return src;
   }

   using Periph<UartReg,BASE>::reg;

private:
   static const uint8_t RX_BUFFER_SIZE = 31;

   uint8_t rx_index{0};
   uint8_t rx_buffer[RX_BUFFER_SIZE];

   void doStartRx() const
   {
      reg->events_endrx  = 0;
      reg->rxd_amount    = 0;
      reg->tasks_startrx = 1;
      reg->rxd_ptr       = uint32_t(rx_buffer) ^ 0x20800000;
   }
};

template <unsigned PIN_RXD,
          unsigned PIN_TXD=PIN_NULL, // Default disconnected
          unsigned PIN_CTS=PIN_NULL, // Default disconnected
          unsigned PIN_RTS=PIN_NULL> // Default disconnected
using Uart0 = UartInstance<0x40002000,PIN_RXD,PIN_TXD,PIN_CTS,PIN_RTS>;

template <unsigned PIN_RXD,
          unsigned PIN_TXD=PIN_NULL, // Default disconnected
          unsigned PIN_CTS=PIN_NULL, // Default disconnected
          unsigned PIN_RTS=PIN_NULL> // Default disconnected
using Uart1 = UartInstance<0x40028000,PIN_RXD,PIN_TXD,PIN_CTS,PIN_RTS>;

} // namespace nRF52

} // namespace MTL

