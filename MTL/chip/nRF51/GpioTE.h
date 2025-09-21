//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \file GpioTE.h
// \brief General Purpose I/O tasks and events
//
// NORDIC Semiconductor
// nRF51
//
// Data source Nordic Semiconductor "nRF51 Series Reference Manual Version 3.0"

#include "MTL/Periph.h"

#ifndef MTL_NRF51_GPIO_TE_H
#define MTL_NRF51_GPIO_TE_H

#include "MTL/Periph.h"

namespace MTL {

namespace nRF51 {

enum
{
   GPIO_TE_EVENT  = 1<<0,
   GPIO_TE_TASK   = 3<<0,

   GPIO_TE_RISE   = 1<<16,
   GPIO_TE_FALL   = 2<<16,
   GPIO_TE_TOGGLE = 3<<16,

   GPIO_TE_INIT1  = 3<<20
};

union GpioTEReg
{
   REG_ARRAY(0x000, out, 4);
   REG_ARRAY(0x100, in,  4);
   REG(      0x17C, port);
   REG(      0x300, inten);
   REG(      0x304, intenset);
   REG(      0x308, intenclr);
   REG_ARRAY(0x510, config, 4);
};

template <unsigned INDEX>
class GpioTE : public Periph<GpioTEReg,0x40006000>
{
public:
   GpioTE(unsigned pin, uint32_t flags)
   {
      reg->config[INDEX] = (pin << 8) | flags;
   }

   void task()
   {
      reg->out[INDEX] = 1;
   }

   uint32_t getTask()
   {
      return uint32_t(&reg->out[INDEX]);
   }
};

} // namespace nRF51

} // namespace MTL

#endif // MTL_NRF51_GPIO_TE_H
