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
//
// NORDIC Semiconductor
// nRF51

#ifndef MTL_NRF51_PPI_H
#define MTL_NRF51_PPI_H

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

#endif
