//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//
// NORDIC Semiconductor
// nRF52
// Non-volatile memory controller
//

#pragma once

#include "MTL/Periph.h"

namespace MTL {

namespace nRF52 {

union NvmcReg
{
   REG(0x400, ready);
   REG(0x408, ready_next);
   REG(0x504, config);
   REG(0x508, erase_page);
   REG(0x508, erase_pcr1);
   REG(0x50C, erase_all);
   REG(0x510, erase_pcr0);
   REG(0x514, erase_uicr);
   REG(0x518, erase_page_partial);
   REG(0x51C, erase_page_partial_cfg);
   REG(0x540, icache_cnf);
   REG(0x548, ihit);
   REG(0x54C, imiss);
};

class Nvmc : public Periph<NvmcReg,0x4001E000>
{
public:
   Nvmc() = default;

   void enableICache(bool profile = false)
   {
      if (profile)
         reg->icache_cnf = (1<<8) | (1<<0);
      else
         reg->icache_cnf = 1<<0;
   }

   uint32_t icacheHit() const { return reg->ihit; }

   uint32_t icacheMiss() const { return reg->imiss; }
};

} // namespace nRF52

} // namespace MTL

