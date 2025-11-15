//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2040 DMA peripheral

#pragma once

#include "MTL/Periph.h"

#include "Resets.h"

namespace MTL {

struct DmaReg
{
   struct Channel
   {
      uint32_t read_addr;
      uint32_t write_addr;
      uint32_t trans_count;
      uint32_t ctrl__trig;

      uint32_t al1_ctrl;
      uint32_t al1_read_addr;
      uint32_t al1_write_addr;
      uint32_t al1_trans_count__trig;

      uint32_t al2_ctrl;
      uint32_t al2_trans_count;
      uint32_t al2_read_addr;
      uint32_t al2_write_addr__trig;

      uint32_t al3_ctrl;
      uint32_t al3_write_addr;
      uint32_t al3_trans_count;
      uint32_t al3_read_addr__trig;
   };

   struct Debug
   {
      uint32_t ctdreq;
      uint32_t tcr;
      uint8_t  pad[0x38];
   };

   Channel  ch[12];
   uint8_t  pad1[0x100];
   uint32_t intr;
   uint32_t inte0;
   uint32_t intf0;
   uint32_t ints0;
   uint32_t pad2;
   uint32_t inte1;
   uint32_t intf1;
   uint32_t ints1;
   uint32_t timer[4];
   uint32_t multi_chan_trigger;
   uint32_t sniff_ctrl;
   uint32_t sniff_data;
   uint32_t pad3;
   uint32_t fifo_levels;
   uint32_t chan_abort;
   uint32_t n_channels;
   uint8_t  pad4[0x3B8];

   Debug    dbg[12];
};

class Dma : public Periph<DmaReg, 0x50000000>
{
public:
   enum DataSize { ONE_BYTE = 0b00, TWO_BYTE = 0b01, FOUR_BYTE = 0b10 };

   Dma() = default;

   //! Allocate the next free channel
   signed allocCH()
   {
      if (free_channel == NUM_CHANNEL)
         return -1;

      return free_channel++;
   }

   static const unsigned DREQ_UNPACED = 0x3F;

   //! Program a channel
   void CH_prog(unsigned       cd,
                unsigned       chain_cd,
                volatile void* read_addr,
                bool           read_incr,
                volatile void* write_addr,
                bool           write_incr,
                unsigned       trans_count,
                DataSize       log2_data_size,
                unsigned       dreq = DREQ_UNPACED)
   {
      uint32_t ctrl = (dreq                 << 15) |
                      (chain_cd             << 11) |
                      ((write_incr ? 1 : 0) <<  5) |
                      ((read_incr  ? 1 : 0) <<  4) |
                      (log2_data_size       <<  2) |
                      (/* EN */ 1           <<  0);

      // Setup without triggering
      reg->ch[cd].read_addr   = uint32_t(read_addr);
      reg->ch[cd].write_addr  = uint32_t(write_addr);
      reg->ch[cd].trans_count = trans_count;
      reg->ch[cd].al1_ctrl    = ctrl;
   }

   void CH_setReadAddr(unsigned cd, volatile void* addr)
   {
      reg->ch[cd].read_addr = uint32_t(addr);
   }

   volatile uint32_t* CH_getReadRegAddr(unsigned cd) const { return &reg->ch[cd].read_addr; }

   void CH_setWriteAddr(unsigned cd, volatile void* addr)
   {
      reg->ch[cd].write_addr = uint32_t(addr);
   }

   bool CH_isIrq(unsigned cd, unsigned irq_n) const
   {
      return irq_n == 0 ? getBit(reg->ints0, cd)
                        : getBit(reg->ints1, cd);
   }

   void CH_clrIrq(unsigned cd, unsigned irq_n)
   {
      if (irq_n == 0)
         reg->ints0 = reg->ints0;
      else
         reg->ints1 = reg->ints1;
   }

   void CH_enableIrq(unsigned cd, unsigned irq_n)
   {
      if (irq_n == 0)
         setBit(reg->inte0, cd, 1);
      else
         setBit(reg->inte1, cd, 1);
   }

   //! Start a channel
   void CH_start(unsigned cd)
   {
      setBit(reg->ch[cd].ctrl__trig, /* EN_BIT */ 0, 1);
   }

   //! Stop a channel
   void CH_stop(unsigned cd)
   {
      setBit(reg->ch[cd].al1_ctrl, /* EN_BIT */ 0, 0);
   }

private:
   static const unsigned NUM_CHANNEL = 12;

   static uint8_t free_channel;
};

} // namespace MTL
