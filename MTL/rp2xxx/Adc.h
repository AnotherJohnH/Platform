//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2xxx ADC peripheral

#pragma once

#include <unistd.h>

#include "MTL/Periph.h"

#include "MTL/chip/PadsBank.h"

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
