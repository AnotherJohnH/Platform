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
//
// NORDIC Semiconductor
// nRF52
// SPI Master peripheral
//

#ifndef MTL_NRF52_SPI_M_H
#define MTL_NRF52_SPI_M_H

#include "MTL/Periph.h"

namespace MTL {

union SpiMReg
{
   REG(0x010, tasks_start);
   REG(0x014, tasks_stop);
   REG(0x01C, tasks_suspend);
   REG(0x020, tasks_resume);
   REG(0x104, events_stopped);
   REG(0x110, events_endrx);
   REG(0x118, events_end);
   REG(0x120, events_endtx);
   REG(0x14C, events_started);
   REG(0x200, shorts);
   REG(0x304, intenset);
   REG(0x308, intenclr);
   REG(0x400, stallstat);
   REG(0x500, enable);
   REG(0x508, psel_sck);
   REG(0x50C, psel_mosi);
   REG(0x510, psel_miso);
   REG(0x514, psel_csn);
   REG(0x524, frequency);
   REG(0x534, rxd_ptr);
   REG(0x538, rxd_maxcnt);
   REG(0x53C, rxd_amount);
   REG(0x540, rxd_list);
   REG(0x544, txd_ptr);
   REG(0x548, txd_maxcnt);
   REG(0x54C, txd_amount);
   REG(0x550, txd_list);
   REG(0x554, config);
   REG(0x560, iftiming_rxdelay);
   REG(0x564, iftiming_csndur);
   REG(0x568, csnpol);
   REG(0x56C, pseldcx);
   REG(0x570, dcxcnt);
   REG(0x5C0, orc);
};

template<uint32_t BASE, unsigned PIN_SCK, unsigned PIN_MOSI, unsigned PIN_MISO>
class SpiMInstance : public Periph<SpiMReg,BASE>
{
public:
   SpiMInstance(unsigned frequency)
   {
      reg->enable = 7;

      switch(frequency)
      {
      case   125000: reg->frequency = 0x02000000; break;
      case   250000: reg->frequency = 0x04000000; break;
      case   500000: reg->frequency = 0x08000000; break;
      case  1000000: reg->frequency = 0x10000000; break;
      case  2000000: reg->frequency = 0x20000000; break;
      case  4000000: reg->frequency = 0x40000000; break;
      case  8000000: reg->frequency = 0x80000000; break;
      case 16000000: reg->frequency = 0x0A000000; break;
      case 32000000: reg->frequency = 0x14000000; break;

      // Unsupported freq => 1 MHz
      default:       reg->frequency = 0x10000000; break;
      }

      reg->psel_sck  = PIN_SCK;
      reg->psel_mosi = PIN_MOSI;
      reg->psel_miso = PIN_MISO;
      reg->psel_csn  = nRF52::PIN_NULL;
   }

   void setTxLength(uint32_t bytes_16)
   {
      reg->txd_maxcnt = bytes_16;
   }

   void setTxData(const volatile uint8_t* buffer)
   {
      reg->txd_ptr = uint32_t(buffer);
   }

   void setRxLength(uint32_t bytes_16)
   {
      reg->rxd_maxcnt = bytes_16;
   }

   void setRxData(volatile uint8_t* buffer)
   {
      reg->rxd_ptr = uint32_t(buffer);
   }

   void start()
   {
      reg->tasks_start = 1;
   }

   void stop()
   {
      reg->tasks_stop = 1;
   }

private:
   using Periph<SpiMReg,BASE>::reg;
};

template<unsigned PIN_SCK, unsigned PIN_MOSI, unsigned PIN_MISO>
using SpiM0 = SpiMInstance<0x40003000,PIN_SCK,PIN_MOSI,PIN_MISO>;

template<unsigned PIN_SCK, unsigned PIN_MOSI, unsigned PIN_MISO>
using SpiM1 = SpiMInstance<0x40004000,PIN_SCK,PIN_MOSI,PIN_MISO>;

template<unsigned PIN_SCK, unsigned PIN_MOSI, unsigned PIN_MISO>
using SpiM2 = SpiMInstance<0x40023000,PIN_SCK,PIN_MOSI,PIN_MISO>;

template<unsigned PIN_SCK, unsigned PIN_MOSI, unsigned PIN_MISO>
using SpiM3 = SpiMInstance<0x4002F000,PIN_SCK,PIN_MOSI,PIN_MISO>;

} // namespace MTL

#endif
