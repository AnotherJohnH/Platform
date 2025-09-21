//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC11U24 USART
//
// Data source NXP document "LPC11U3X-2X-1X User Manual UM10462"

#pragma once

#include "MTL/Periph.h"

#include "IoCon.h"
#include "Pins.h"
#include "SysCon.h"

namespace MTL {

union USARTReg
{
   REG(0x000, rbr_thr_dll);
   REG(0x004, ier_dlm);
   REG(0x008, iir_fcr);
   REG(0x00C, lcr);
   REG(0x010, mcr);
   REG(0x014, lsr);
   REG(0x018, msr);
   REG(0x01C, scr);
   REG(0x020, acr);
   REG(0x024, icr);
   REG(0x028, fdr);
   REG(0x02C, osr);
   REG(0x030, ter);
   REG(0x040, hden);
   REG(0x048, scictrl);
   REG(0x04C, rs485ctrl);
   REG(0x050, rs485adrmatch);
   REG(0x054, rs485dly);
   REG(0x058, syncctrl);
};

class USART : public Periph<USARTReg,0x40008000>
{
public:
   enum Baud
   {
      BAUD_9600,
      BAUD_19200,
      BAUD_38400,
      BAUD_57600,
      BAUD_74800,
      BAUD_115200
   };

   enum Parity
   {
      NONE,
      ODD,
      EVEN
   };

   USART(Baud     baud,
         unsigned data_bits = 8,
         Parity   parity    = NONE,
         unsigned stop_bits = 1)
   {
      IoCon  iocon;
      SysCon syscon;

      // 1. Configure Pins
      iocon.config(PIN_0_18, IoCon::Func(1), IoCon::PULL_NONE); // RXD
      iocon.config(PIN_0_19, IoCon::Func(1), IoCon::PULL_NONE); // TXD

      //iocon.config(PIN_1_26, IoCon::Func(2), IoCon::PULL_NONE); // RXD
      //iocon.config(PIN_1_27, IoCon::Func(2), IoCon::PULL_NONE); // TXD

      // 2. Enable USART Power and peripheral clock
      syscon.enableAHBClkCtrl(12);

      // 3. USART clock divider (no division)
      syscon.reg->uartclkdiv = 1;

      // 4. USART oversampling register (x8)
      reg->osr = (8 - 1) << 4;

      static const uint32_t FRAC_1_1_4   = ( 4 << 4) |  1;
      static const uint32_t FRAC_1_13_14 = (14 << 4) | 13;
      static const uint32_t FRAC_1_12_14 = (14 << 4) | 12;

      // 4. USART divisor and fraction
      unsigned divisor;

      switch(baud)
      {
      default: // Fall through to 9600
      case BAUD_9600:   divisor = 500; reg->fdr = FRAC_1_1_4;   break;
      case BAUD_19200:  divisor = 250; reg->fdr = FRAC_1_1_4;   break;
      case BAUD_38400:  divisor = 125; reg->fdr = FRAC_1_1_4;   break;
      case BAUD_57600:  divisor =  54; reg->fdr = FRAC_1_13_14; break;
      case BAUD_74800:  divisor =  42; reg->fdr = FRAC_1_12_14; break;
      case BAUD_115200: divisor =  28; reg->fdr = FRAC_1_12_14; break;
      }

      reg->lcr.setBit(7); // DLAB - Enable access to divisor register
      reg->rbr_thr_dll = divisor & 0xFF;
      reg->ier_dlm     = divisor >> 8;
      reg->lcr.clrBit(7); // DLAB - Disable access to divisor register

      // 5. Configure protocol
      uint32_t lcr;
      switch (data_bits)
      {
      case 5: lcr = 0; break;
      case 6: lcr = 1; break;
      case 7: lcr = 2; break;
      default: case 8: lcr = 3; break;
      }

      if (stop_bits == 2)
         lcr |= 1<<2;

      switch (parity)
      {
      case NONE: break;
      case ODD:  lcr |= (0<<4) | (1<<3); break;
      case EVEN: lcr |= (1<<4) | (1<<3); break;
      }
      reg->lcr = lcr;

      // 6. Enable FIFOs
      reg->iir_fcr.setBit(0);
   }

   void tx(uint8_t data)
   {
      while(reg->lsr & (1<<5) == 0);

      reg->rbr_thr_dll = data;
   }

   uint8_t rx()
   {
      while((reg->lsr & (1<<0)) == 0);

      return reg->rbr_thr_dll;
   }
};

} // namespace MTL
