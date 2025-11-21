//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//
// NORDIC Semiconductor
// nRF52
// I2S peripheral
//
// XXX - Broken cannot get this peripheral to work
//       Clock signals working but no data tx
//

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union I2SReg
{
   REG(0x000, task_start);
   REG(0x004, task_stop);
   REG(0x104, events_rxptrupd);
   REG(0x108, events_stopped);
   REG(0x114, events_txptrupd);
   REG(0x300, inten);
   REG(0x304, intenset);
   REG(0x308, intenclr);
   REG(0x500, enable);
   REG(0x504, config_mode);
   REG(0x508, config_rxen);
   REG(0x50C, config_txen);
   REG(0x510, config_mcken);
   REG(0x514, config_mckfreq);
   REG(0x518, config_ratio);
   REG(0x51C, config_swidth);
   REG(0x520, config_align);
   REG(0x524, config_format);
   REG(0x528, config_channels);
   REG(0x538, rxd_ptr);
   REG(0x540, txd_ptr);
   REG(0x550, rxtxd_maxcnt);
   REG(0x560, psel_mck);
   REG(0x564, psel_sck);
   REG(0x568, psel_lrck);
   REG(0x56C, psel_sdin);
   REG(0x570, psel_sdout);
};

template<unsigned PIN_SDOUT, unsigned PIN_MCK>
class I2S : public Periph<I2SReg,0x40025000>
{
public:
   I2S(unsigned freq)
   {
      reg->config_mode     = 0;          // Master
      reg->config_rxen     = 0;          // No RX
      reg->config_txen     = 1;          // TX enabled
      reg->config_mcken    = 1;          // Enable master clock?
      reg->config_mckfreq  = 0x020C0000; // 4 MHz
      reg->config_ratio    = 0;          // LRCK = MCK / 32
      reg->config_swidth   = 1;          // 16 bit
      reg->config_align    = 0;          // Left aligned
      reg->config_format   = 0;          // Aligned
      reg->config_channels = 0;          // Stereo

      reg->psel_mck   = 1<<31;
      reg->psel_sck   = PIN_MCK; // Disconnected
      reg->psel_lrck  = 1<<31;   // Disconnected
      reg->psel_sdin  = 1<<31;   // Disconnected
      reg->psel_sdout = PIN_SDOUT;
   }

   void setTxLength(uint32_t words_14)
   {
      reg->rxtxd_maxcnt = words_14;
   }

   void setTxData(volatile const uint32_t* buffer)
   {
      reg->txd_ptr = uint32_t(buffer);
   }

   void enable()
   {
      reg->enable = 1;
   }

   void start()
   {
      //reg->events_txptrupd = 0;
      reg->task_start = 1;
   }

   void stop()
   {
      reg->task_stop = 1;
   }

   bool isTxPtrUpd() const { return reg->events_txptrupd; }

   void clearTxPtrUpd()
   {
      reg->events_txptrupd = 0;

      // XXX no idea what this is about. nRF SDK does it nothing in the erata
      volatile uint32_t dummy = reg->events_txptrupd; (void) dummy;
   }

private:
   using Periph<I2SReg,0x40025000>::reg;
};

} // namespace MTL

