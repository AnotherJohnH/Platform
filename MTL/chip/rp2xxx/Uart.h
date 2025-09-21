//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2040/RP2350 UART peripheral (an instance of Arm PL011)

#pragma once

#include "MTL/Periph.h"
#include "MTL/core/NVIC.h"

#include "STB/Fifo.h"

#include "PadsBank.h"
#include "Pll.h"
#include "IoBank.h"
#include "Irq.h"
#include "Pins.h"

namespace MTL {

namespace UART { enum Parity { NONE, ODD, EVEN }; }

struct UartReg
{
   uint32_t dr;
   uint32_t rsr;
   uint32_t pad[4];
   uint32_t tfr;
   uint32_t pad2;

   uint32_t ilpr;
   uint32_t ibrd;
   uint32_t fbrd;
   uint32_t lcr_h;

   uint32_t cr;
   uint32_t ifls;    //!< Interrupt FIFO level select
   uint32_t imsc;    //!< Interrupt mask set/clear
   uint32_t ris;     //!< Raw interrupt status

   uint32_t mis;     //!< Masked interrupt status
   uint32_t icr;     //!< Interrupt clear
   uint32_t dmacr;
};

using UartFifo = STB::Fifo<uint8_t, /* LOG2_BUFFER_SIZE */ 9>;

//! Base class with RX/TX buffers for each UART instance
template <unsigned INDEX>
class UartBuffers
{
public:
   UartBuffers() = default;

protected:
   static UartFifo rx_buffer;
   static UartFifo tx_buffer;
};

template <uint32_t BASE_ADDRESS, unsigned INDEX, unsigned TX_PIN, unsigned RX_PIN>
class Uart
   : public Periph<UartReg, BASE_ADDRESS>
   , public UartBuffers<INDEX>
{
public:
   Uart() = default;

   Uart(unsigned     baud,
        unsigned     data_bits = 8,
        UART::Parity parity    = UART::NONE,
        unsigned     stop_bits = 1,
        bool         pull_up   = false)
   {
      IoBank io_bank;

      uint8_t rx_pad_ctrl = PadsBank::IE | PadsBank::OD;
      if (pull_up)
         rx_pad_ctrl |= PadsBank::PULL_UP;

      io_bank.setFunc(TX_PIN, IoBank::UART, PadsBank::DRIVE_2MA);
      io_bank.setFunc(RX_PIN, IoBank::UART, rx_pad_ctrl);

      // Setup baud rate
      uint32_t divisor = (8 * PllSys().getFreq()) / baud;
      this->reg->ibrd = divisor >> 7;
      this->reg->fbrd = ((divisor & 0x7F) + 1) >> 1;

      // Setup format
      uint32_t lcr_h = 0;
      this->setField(lcr_h, 6, 5, data_bits - 5);     // WLEN
      this->setBit(  lcr_h, 4, 1);                    // FEN
      this->setBit(  lcr_h, 3, stop_bits - 1);        // STP2
      this->setBit(  lcr_h, 2, parity == UART::EVEN); // EPS
      this->setBit(  lcr_h, 1, parity != UART::NONE); // PEN
      this->reg->lcr_h = lcr_h;

      // Enable UART TX and RX
      this->reg->cr = (1 << 9) |  // TXEN
                      (1 << 8) |  // RXEN
                      (1 << 0);   // EN

      // Enable interrupts
      this->reg->ifls = (/* RXIFLSEL (7/8 full) */ 4 << 3) | /* TXIFLSEL (1/8 empty) */ 0;
      this->reg->imsc = RXIM;

      MTL::NVIC<IRQ_UART0 + INDEX>().enable();
   }

   void disableTxBuffer() { use_tx_buffer = false; }

   //! Check if recieve buffers are empty
   bool empty() const
   {
      return this->rx_buffer.empty() && rx_fifo_empty();
   }

   //! Check if transmit buffers are full
   bool full() const
   {
      return this->tx_buffer.full() || tx_fifo_full();
   }

   //! Schedule a byte to be sent
   void tx(uint8_t byte)
   {
      if (this->tx_buffer.empty())
      {
         while(true)
         {
            if (not tx_fifo_full())
            {
               // push into HW FIFO
               this->reg->dr = byte;
               return;
            }

            if (use_tx_buffer) break;
         }

         // push into TX buffer and exanble TX FIFO low irq
         this->tx_buffer.push(byte);
         this->reg->imsc |= TXIM;
         return;
      }

      // Block when transmit buffer is full
      while(this->tx_buffer.full());

      this->tx_buffer.push(byte);
   }

   //! Return the next byte recieved
   uint8_t rx()
   {
      if (not this->rx_buffer.empty())
      {
         uint8_t byte = this->rx_buffer.back();
         this->rx_buffer.pop();
         return byte;
      }

      // Block when hardware FIFO empty
      while(rx_fifo_empty());

      return this->reg->dr;
   }

   //! Interrupt handler
   void irq()
   {
      if (this->reg->mis & RXIM)
      {
         // Spool HW FIFO into RX buffer
         while(not rx_fifo_empty())
         {
            if (not this->rx_buffer.full())
            {
               this->rx_buffer.push(this->reg->dr);
            }
            // else { XXX DATA LOSS }
         }
      }

      if (this->reg->mis & TXIM)
      {
         // Spool TX buffer into HW FIFO
         while(not tx_fifo_full())
         {
            if (this->tx_buffer.empty())
            {
               // Disable TX FIFO low IRQ
               this->reg->imsc &= ~TXIM;
               break;
            }

            this->reg->dr = this->tx_buffer.back();
            this->tx_buffer.pop();
         }
      }
   }

   static constexpr unsigned TX = TX_PIN;
   static constexpr unsigned RX = RX_PIN;

private:
   //! Check if RX FIFO is empty
   bool rx_fifo_empty() const
   {
      return this->getBit(this->reg->tfr, 4) != 0;
   }

   //! Check if TX FIFO is full
   bool tx_fifo_full() const
   {
      return this->getBit(this->reg->tfr, 5) != 0;
   }

   // Interrupt masks
   static const uint32_t TXIM = 1 << 5;
   static const uint32_t RXIM = 1 << 4;

   bool use_tx_buffer{true};
};

#if defined(MTL_RP2040)

using Uart0_P1_P2   = Uart<0x40034000, 0, MTL::IO_PIN_0,  MTL::IO_PIN_1>;
using Uart0_P16_P17 = Uart<0x40034000, 0, MTL::IO_PIN_12, MTL::IO_PIN_13>;
using Uart0_P21_P22 = Uart<0x40034000, 0, MTL::IO_PIN_16, MTL::IO_PIN_17>;

using Uart1_P6_P7   = Uart<0x40038000, 1, MTL::IO_PIN_4,  MTL::IO_PIN_5>;
using Uart1_P11_P12 = Uart<0x40038000, 1, MTL::IO_PIN_8,  MTL::IO_PIN_9>;
using Uart1_P26_P27 = Uart<0x40038000, 1, MTL::IO_PIN_20, MTL::IO_PIN_21>;

#elif defined(MTL_RP2350)

using Uart0_P1_P2   = Uart<0x40070000, 0, MTL::IO_PIN_0,  MTL::IO_PIN_1>;
using Uart0_P16_P17 = Uart<0x40070000, 0, MTL::IO_PIN_12, MTL::IO_PIN_13>;
using Uart0_P21_P22 = Uart<0x40070000, 0, MTL::IO_PIN_16, MTL::IO_PIN_17>;

using Uart1_P6_P7   = Uart<0x40078000, 1, MTL::IO_PIN_4,  MTL::IO_PIN_5>;
using Uart1_P11_P12 = Uart<0x40078000, 1, MTL::IO_PIN_8,  MTL::IO_PIN_9>;
using Uart1_P26_P27 = Uart<0x40078000, 1, MTL::IO_PIN_20, MTL::IO_PIN_21>;

#endif

} // namespace MTL
