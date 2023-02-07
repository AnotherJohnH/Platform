//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

// nRF51

#ifndef MTL_NRF51_SPI_SLAVE_H
#define MTL_NRF51_SPI_SLAVE_H

#include "MTL/Periph.h"
#include "MTL/Gpio.h"

namespace MTL {

namespace nRF51 {

union SpiSlaveReg
{
   REG(0x024, task_acquire);
   REG(0x028, task_release);
   REG(0x104, event_end);
   REG(0x128, event_acquired);
   REG(0x200, shorts);
   REG(0x300, inten);
   REG(0x304, intenset);
   REG(0x308, intenclr);
   REG(0x400, semstat);
   REG(0x440, status);
   REG(0x500, enable);
   REG(0x508, pselsck);
   REG(0x50C, pselmiso);
   REG(0x510, pselmosi);
   REG(0x514, pselcsn);
   REG(0x534, rxdptr);
   REG(0x538, maxrx);
   REG(0x53c, amountrx);
   REG(0x544, txdptr);
   REG(0x548, maxtx);
   REG(0x54c, amounttx);
   REG(0x554, config);
   REG(0x55c, def);
   REG(0x5C0, orc);
};

template <unsigned PSELCSN,
          unsigned PSELMOSI,
          unsigned PSELMISO,
          unsigned PSELSCK>
class SpiSlave : public Periph<SpiSlaveReg,0x40004000>
{
private:
   Gpio::In<1,PSELCSN>   csn;
   Gpio::In<1,PSELMOSI>  mosi;
   Gpio::Out<1,PSELMISO> miso;
   Gpio::In<1,PSELSCK>   sck;

public:
   // Tasks
   void acquire()  { reg->task_acquire = 1; }
   void release()  { reg->task_release = 1; }

   SpiSlave()
   {
      reg->shorts   = 1<<2;     // Enable end acquire

      reg->pselcsn  = PSELCSN;
      reg->pselmiso = PSELMISO;
      reg->pselmosi = PSELMOSI;
      reg->pselsck  = PSELSCK;

      reg->config   = (0<<0) |  // MSB out first
                      (0<<1) |  // sample on leading edge, tx on trailing edge
                      (0<<2);   // sck is active high

      setDefaultOut(0x00);
      setOverReadOut(0x00);

      reg->enable = 2;        // Enable SPI slave
   }

   void setDefaultOut(uint8_t data)
   {
      reg->def = data;
   }

   void setOverReadOut(uint8_t data)
   {
      reg->orc = data;
   }

   void setRxBuffer(uint8_t* start, uint8_t size)
   {
      acquire();
      while(!reg->event_acquired);
      reg->rxdptr = uint32_t(start);
      reg->maxrx  = size;
      release();
   }

   void setTxBuffer(const uint8_t* start, uint8_t size)
   {
      acquire();
      while(!reg->event_acquired);
      reg->txdptr = uint32_t(start);
      reg->maxtx  = size;
      release();
   }

   void setTxPtr(const uint8_t* start)
   {
      reg->txdptr = uint32_t(start);
   }

   void setTxLength(uint8_t size)
   {
      reg->maxtx = size;
   }
};

} // namespace nRF51

} // namespace MTL

#endif
