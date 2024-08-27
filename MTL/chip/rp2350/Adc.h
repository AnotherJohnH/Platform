//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

// \brief RP2350 ADC peripheral

#pragma once

#include "MTL/Periph.h"

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
class Adc
   : public Periph<AdcReg, 0x400a0000>
{
public:
   Adc() = default;

   void enable(bool enable_)
   {
      reg->cs = enable_ ? 1 : 0;
   }

   //! Start continous sampling
   void startCont(unsigned channel)
   {
      enable(true);
      reg->cs |= (channel << 12) | (1 << 3);
   }

   //! Start single sample
   void startOnce(unsigned channel)
   {
      enable(true);
      reg->cs |= (channel << 12) | (1 << 2);
   }

   //! Get latest sample
   unsigned rawResult()
   {
      return reg->result;
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
};

} // namespace MTL
