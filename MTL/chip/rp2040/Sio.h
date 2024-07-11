//------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
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

// \brief RP2040 Single-cycle I/O

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
   uint32_t gpio_out_set;
   uint32_t gpio_out_clr;
   uint32_t gpio_out_xor;
   uint32_t gpio_oe;
   uint32_t gpio_oe_set;
   uint32_t gpio_oe_clr;
   uint32_t gpio_oe_xor;
   uint32_t gpio_hi_out;
   uint32_t gpio_hi_out_set;
   uint32_t gpio_hi_out_clr;
   uint32_t gpio_hi_out_xor;
   uint32_t gpio_hi_oe;
   uint32_t gpio_hi_oe_set;
   uint32_t gpio_hi_oe_clr;
   uint32_t gpio_hi_oe_xor;
   uint32_t fifo_st;
   uint32_t fifo_wr;
   uint32_t fifo_rd;
   uint32_t spinlock_st;
   uint32_t div_udividend;
   uint32_t div_udivisor;
   uint32_t div_sdividend;
   uint32_t div_sdivisor;
   uint32_t div_quotient;
   uint32_t div_remainder;
   uint32_t div_csr;

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
