//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

// \brief RP2040/RP2350 ADC peripheral

#pragma once

#include <unistd.h>

#include "MTL/Periph.h"

#include "PadsBank.h"

namespace MTL {

struct AdcReg
{
   uint32_t cs;
   uint32_t result;
   uint32_t fcs;
   uint32_t fio;
   uint32_t div;
   uint32_t intr;
   uint32_t inte;
   uint32_t intf;
   uint32_t ints;
};

//! Analogue to digital convertor
template <unsigned CHANNEL>
class Adc
#if defined(MTL_RP2040)
   : public Periph<AdcReg, 0x4004C000>
#elif defined(MTL_RP2350)
   : public Periph<AdcReg, 0x400A0000>
#endif
{
public:
   Adc()
   {
       if (CHANNEL <= 3)
       {
          MTL::PadsBank pads_bank;

          pads_bank.setAnalogIn(MTL::IO_PIN_26 + CHANNEL);
       }
   }

   bool ready() const { return (reg->cs & (1<<8)) != 0; }

   void enable(bool enable_)
   {
      reg->cs = enable_ ? 0b0001 : 0b0000;
   }

   //! Start continous sampling
   void startCont()
   {
      reg->cs = (CHANNEL << 12) | 0b1001;
   }

   //! Start single sample
   void startOnce()
   {
      reg->cs = (CHANNEL << 12) | 0b0101;
   }

   //! Get latest sample
   unsigned rawResult() const
   {
      return reg->result;
   }

   //! Read once and return the raw result
   unsigned readOnce()
   {
      enable(true);

      while(not ready());
      startOnce();

      usleep(2);
      while(not ready());
      unsigned result = rawResult();

      enable(false);

      return result;
   }

   //! Get latest sample
   unsigned scaledResult(unsigned range)
   {
      uint32_t value = rawResult();

      if (value > 0)
         value = value * range / 4096;

      if (value > (range - 1))
         value = range - 1;

      return value;
   }

   operator uint32_t() { return readOnce(); }
};

} // namespace MTL
