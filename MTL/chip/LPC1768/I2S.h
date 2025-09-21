//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC1768 I2S
//
// Data source NXP document "LPC17xx User Manual UM10360"

#pragma once

#include "MTL/Periph.h"

#include "SysCon.h"
#include "PinCon.h"

#include <cassert>

namespace MTL {

union I2SReg
{
   REG(0x000, i2sdao);
   REG(0x004, i2sdai);
   REG(0x008, i2stxfifo);
   REG(0x00C, i2srxfifo);
   REG(0x010, i2state);
   REG_ARRAY(0x014, i2sdma, 2);
   REG(0x01C, i2sirq);
   REG(0x020, i2stxrate);
   REG(0x024, i2srxrate);
   REG(0x028, i2stxbitrate);
   REG(0x02C, i2srxbitrate);
   REG(0x030, i2stxmode);
   REG(0x034, i2srxmode);
};

class I2S : public Periph<I2SReg,0x400A8000>
{
public:
   //! Enable I2S feature in TX master mode
   //
   //! This will enable the pin for the I2S_SDA function
   //! on mbed-LPC1768 this is DIP5
   I2S()
   {
      // Power up
      SysCon().pconp(27, true);

      // Select PCLK/1 clock
      SysCon().pclksel1(22, 1);

      // IS2TX_SDA
      PinCon().config(PIN_0_9, 1, PinCon::PULL_NONE);

      // TXCLKSEL=0, TX4PIN=0, TXMCENA=0
      reg->i2stxmode = 0;

      // TX in master mode
      reg->i2sdao.setBit(5, false);
   }

   static unsigned getFifoLen() { return 8; }

   //! Enable pin for IS2TX_WS function
   //
   //! On mbed-LPC1768 this is DIP6
   void enablePin_TX_WS()
   {
      PinCon().config(PIN_0_8, 1, PinCon::PULL_NONE);
   }

   //! Enable pin for IS2TX_CLK function
   //
   //! On mbed-LPC1768 this is DIP7
   void enablePin_TX_CLK()
   {
      PinCon().config(PIN_0_7, 1, PinCon::PULL_NONE);
   }

   //! Select the sample format
   void setTxFormat(unsigned bits, bool mono)
   {
      assert((bits == 32) || (bits == 16) || (bits == 8));

      reg->i2sdao.setField( 1, 0, (bits/8) - 1);
      reg->i2sdao.setField(14, 6, bits - 1);
      reg->i2sdao.setBit(2, mono);
   }

   //! Set transmit bit rate
   //
   //! Frequency is (numerator * PCLK) / (denominator * 2)
   //! the frequency cannot be greater than PCLK/2
   void setTxMulDiv(unsigned numerator, unsigned denominator)
   {
      reg->i2stxrate    = (numerator<<8) | denominator;
      reg->i2stxbitrate = 0;
   }

   //! Disable accesses on FIFOs, places transmit channel in mute
   void setTxStop(bool state)
   {
      reg->i2sdao.setBit(3, state);
   }

   //! Asynchronously resets the transmit channel and FIFO
   void setTxReset(bool state)
   {
      reg->i2sdao.setBit(4, state);
   }

   //! When set the channel on tranmits zeros
   void setTxMute(bool state)
   {
      reg->i2sdao.setBit(15, state);
   }

   //! Write data to the transmit FIFO
   void send(uint32_t data)
   {
      reg->i2stxfifo = data;
   }

   //! Configure DMA
   //
   //! \param n
   //! \param tx
   //! \param depth
   void configDMA(unsigned n, bool tx, unsigned depth)
   {
      reg->i2sdma[n].setField(tx ? 19 : 11,
                              tx ? 16 :  8,
                              depth);

      reg->i2sdma[n].setBit(  tx ?  1 : 0, true);
   }

   //! Return address of transmit FIFO
   volatile uint32_t* getTxFifoAddress()
   {
      return (volatile uint32_t*)&reg->i2stxfifo;
   }

   //! Enable transmit FIFO level interrupt
   void enableTxIRQ(unsigned level)
   {
      reg->i2sirq = (1<<1) | (level<<16);
   }

   //! Disable transmit FIFO level interrupt
   void disableTxIRQ()
   {
      reg->i2sirq.clrBit(1);
   }
};

} // namespace MTL
