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
// Non-volatile memory controller
//

#ifndef MTL_NRF52_NVMC_H
#define MTL_NRF52_NVMC_H

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

#endif
