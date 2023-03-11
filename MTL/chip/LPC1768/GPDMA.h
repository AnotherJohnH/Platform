//------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
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

// \brief NXP LPC1768 General Purpose DMA
//
// Data source NXP document "LPC17xx User Manual UM10360"

#pragma once

#include "MTL/Periph.h"

#include "SysCon.h"

namespace MTL {

class DMA
{
public:
   enum Burst
   {
      BURST1   = 0,
      BURST4   = 1,
      BURST8   = 2,
      BURST16  = 3,
      BURST32  = 4,
      BURST64  = 5,
      BURST128 = 6,
      BURST256 = 7
   };

   enum Width { WIDTH8 = 0, WIDTH16 = 1, WIDTH32 = 2};

   DMA() = default;

   void setSrc(volatile const void* ptr) volatile
   {
      src_addr = (uint8_t*)(ptr);
   }

   void setDest(volatile void* ptr) volatile
   {
      dest_addr = (uint8_t*)(ptr);
   }

   void setNext(volatile DMA* ptr) volatile
   {
      next = ptr;
   }

   void setControl(Burst    src_burst,
                   Width    src_width,
                   bool     src_inc,
                   Burst    dest_burst,
                   Width    dest_width,
                   bool     dest_inc,
                   unsigned size12,
                   bool     irq = false) volatile
   {
      control = (size12         <<  0)
              | (src_burst      << 12)
              | (dest_burst     << 15)
              | (src_width      << 18)
              | (dest_width     << 21)
              | ((src_inc  & 1) << 26)
              | ((dest_inc & 1) << 27)
              | ((irq      & 1) << 31);
   }

private:
   const uint8_t*      src_addr {0};
   uint8_t*            dest_addr {0};
   const volatile DMA* next {nullptr};
   uint32_t            control {0};
};
   
union GPDMAReg
{
   REG(0x000, int_stat);
   REG(0x004, int_tc_stat);
   REG(0x008, int_tc_clear);
   REG(0x00C, err_stat);
   REG(0x010, err_clr);
   REG(0x014, raw_int_tc_stat);
   REG(0x018, raw_int_err_stat);
   REG(0x01C, enbld_chns);
   REG(0x020, soft_b_req);
   REG(0x024, soft_s_req);
   REG(0x028, soft_lb_req);
   REG(0x02C, soft_ls_req);
   REG(0x030, config);
   REG(0x034, sync);

   struct Channel
   {
      DMA                dma;
      Register<uint32_t> config;
      uint8_t            pad[0x20 - 5*4];
   };
 
   REG_TYPE_ARRAY(0x100, Channel, chan, 8);
};

class GPDMA : public Periph<GPDMAReg,0x50004000>
{
public:
   enum Periph
   {
      SSP0_TX = 0,
      SSP0_RX = 1,
      SSP1_TX = 2,
      SSP1_RX = 3,
      ADC     = 4,
      I2S_0   = 5,
      I2S_1   = 6,
      DAC     = 7,

      MEMORY  = 16
   };

   GPDMA()
   {
      // Power up
      SysCon().pconp(29, true);

      // Enable in little endian mode
      reg->config = (0<<1) | (1<<0);
   }

   void configChan(unsigned n,
                   Periph   src_periph,
                   Periph   dest_periph,
                   bool     irq = false)
   {
      clearIrq(n);

      // Configure source and destination types
      unsigned transfer_type;
      if (src_periph == MEMORY)
      {
         transfer_type = dest_periph == MEMORY ? 0b000 : 0b001;
      }
      else
      {
         transfer_type = dest_periph == MEMORY ? 0b010 : 0b011;
      }

      reg->chan[n].config = (0                   <<  0)
                          | ((src_periph  & 0xF) <<  1)
                          | ((dest_periph & 0xF) <<  6)
                          | (transfer_type       << 11)
                          | ((irq & 1)           << 15);
   }

   volatile DMA* getHead(unsigned n)
   {
      return &reg->chan[n].dma;
   }

   void start(unsigned n)
   {
      reg->chan[n].config.setBit(0);
   }

   void clearIrq(unsigned n)
   {
      // Clear any pending error interrupt request
      reg->err_clr.setBit(n);

      // Clear any pending interrupt terminal count request
      reg->int_tc_clear.setBit(n);
   }
};

} // namespace MTL
