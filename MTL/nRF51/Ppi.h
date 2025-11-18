//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//
// NORDIC Semiconductor
// nRF51

#pragma once

#include "MTL/Periph.h"
#include "MTL/nRF51/Irq.h"
#include "MTL/CortexM0/Nvic.h"

namespace MTL {

namespace nRF51 {

union PpiReg
{
   struct Chg
   {
      uint32_t  en;
      uint32_t  dis;
   };

   struct Ch
   {
      uint32_t  eep;
      uint32_t  tep;
   };

   REG_TYPE_ARRAY(0x000, Chg, chg, 4);

   REG(0x500, chen);
   REG(0x504, chenset);
   REG(0x508, chenclr);

   REG_TYPE_ARRAY(0x510, Ch, ch, 16);
};

template <unsigned CHAN>
class Ppi : public Periph<PpiReg,0x4001F000>
{
public:
   void config(uint32_t event, uint32_t task)
   {
      reg->ch[CHAN].eep = event;
      reg->ch[CHAN].tep = task;

      enable();
   }

   void enable()
   {
      reg->chenset = 1<<CHAN;
   }

   void disable()
   {
      reg->chenclr = 1<<CHAN;
   }
};

} // namespace nRF51

} // namespace MTL

