//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2350 Single-cycle I/O

#pragma once

#include "MTL/Periph.h"

namespace MTL {

struct SioReg
{
   uint32_t cpu_id;
   uint32_t gpio_in;
   uint32_t gpio_hi_in;
   uint32_t pad1;
   uint32_t gpio_out;
   uint32_t gpio_hi_out;
   uint32_t gpio_out_set;
   uint32_t gpio_hi_out_set;
   uint32_t gpio_out_clr;
   uint32_t gpio_hi_out_clr;
   uint32_t gpio_out_xor;
   uint32_t gpio_hi_out_xor;
   uint32_t gpio_oe;
   uint32_t gpio_hi_oe;
   uint32_t gpio_oe_set;
   uint32_t gpio_hi_oe_set;
   uint32_t gpio_oe_clr;
   uint32_t gpio_hi_oe_clr;
   uint32_t gpio_oe_xor;
   uint32_t gpio_hi_oe_xor;
   uint32_t fifo_st;
   uint32_t fifo_wr;
   uint32_t fifo_rd;
   uint32_t spinlock_st;

   struct
   {
      uint32_t accum[2];
      uint32_t base[3];
      uint32_t pop_lane[2];
      uint32_t pop_full;
      uint32_t peek_lane[2];
      uint32_t peek_full;
      uint32_t ctrl_lane[2];
      uint32_t accum_add[2];
      uint32_t base_1and0;

   } interp[2];

   uint32_t spinlock[32];

   uint32_t doorbell_out_set;
   uint32_t doorbell_out_clr;
   uint32_t doorbell_in_set;
   uint32_t doorbell_in_clr;
   uint32_t peri_nonsec;
   uint32_t pad2[11];
   uint32_t tmds_ctrl;
   uint32_t tmds_wdata;
   uint32_t tmds_peek_single;
   uint32_t tmds_pop_single;
   uint32_t tmds_pop_double_l0;
   uint32_t tmds_peek_double_l0;
   uint32_t tmds_pop_double_l1;
   uint32_t tmds_peek_double_l1;
   uint32_t tmds_pop_double_l2;
   uint32_t tmds_peek_double_l2;
};

class Sio : public Periph<SioReg, 0xD0000000>
{
public:
   Sio() = default;

   bool try_lock(unsigned index_)
   {
      return reg->spinlock[index_] != 0;
   }

   void unlock(unsigned index_)
   {
      reg->spinlock[index_] = 0;
   }

   bool rxFifoEmpty() const
   {
      return (reg->fifo_st & 0b0001) == 0;
   }

   uint32_t rxFifoPop()
   {
      while(rxFifoEmpty());

      return reg->fifo_rd;
   }

   void rxFifoDrain()
   {
      while(not rxFifoEmpty())
      {
         (void) rxFifoPop();
      }
   }

   bool txFifoFull() const
   {
      return (reg->fifo_st & 0b0010) == 0;
   }

   void txFifoPush(uint32_t data_)
   {
      while(txFifoFull());

      reg->fifo_wr = data_;
   }
};

} // namespace MTL
