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
// PPI peripheral
//

#ifndef MTL_NRF52_PPI_H
#define MTL_NRF52_PPI_H

#include "MTL/Periph.h"

namespace MTL {

namespace nRF52 {

union PpiReg
{
   struct Chg
   {
      uint32_t en;  //!< Enable channel group
      uint32_t dis; //!< Disable channel group
   };

   struct Ch
   {
      uint32_t eep; //!< Channel event end-point
      uint32_t tep; //!< Channel task end-point
   };

   struct Fork
   {
      uint32_t tep; //!< Channel n task end-point
   };

   REG_TYPE_ARRAY(0x000, Chg,  tasks_chg,  6); //!< Channel group enables
   REG(           0x500,       chen         ); //!< Channel enable register
   REG(           0x504,       chenset      ); //!< Channel enable set register
   REG(           0x508,       chenclr      ); //!< Channel enable clear register
   REG_TYPE_ARRAY(0x510, Ch,   ch,        20); //!< Channel n
   REG_ARRAY(     0x800,       chg,        6); //!< Channel group n
   REG_TYPE_ARRAY(0x910, Fork, fork,      32); //!< Channel task end points
};

class Ppi : public Periph<PpiReg,0x4001F000>
{
public:
   Ppi() = default;

   //! Check for channel enablement
   bool isEnabled(unsigned n) const { return ((reg->chen >> n) & 1) != 0; }

   //! Enable channel
   void enable(unsigned n) { reg->chenset = (1 << n); }

   //! Disable channel
   void disable(unsigned n) { reg->chenclr = (1 << n); }

   //! Link an event to a task. \return the allocated channel #
   unsigned link(uint32_t event, uint32_t task, bool enable_ = false)
   {
      static unsigned free_channel = 0;

      if (free_channel == FIRST_FIXED_CHANNEL) return BAD_INDEX;

      unsigned n = free_channel++;

      reg->ch[n].eep = event;
      reg->ch[n].tep = task;

      if (enable_)
         enable(n);
      else
         disable(n);

      return n;
   }

   static const unsigned BAD_INDEX{0xFFFFffff};

private:
   using Periph<PpiReg,0x4001F000>::reg;

   static const unsigned FIRST_FIXED_CHANNEL{20};
};

} // namespace nRF52

} // namespace MTL

#endif
