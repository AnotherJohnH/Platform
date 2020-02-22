//------------------------------------------------------------------------------
// Copyright (c) 2016-2017 John D. Haughton
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

//! \file Yield.h
//! \brief Real-time scheduling

#ifndef PLT_YIELD_H
#define PLT_YIELD_H

#include <cstdint>

//! Platform abstraction layer
namespace PLT {

//!
class Yield
{
public:
   Yield(unsigned freq_hz)
      : period_us(1000000/freq_hz)
   {
      next_event_ticks = getTicksUS() + period_us;
   }

   //! Poll for next tick event
   bool refresh()
   {
      int32_t delta_t = next_event_ticks - getTicksUS();
      if (delta_t > 0) return false;

      next_event_ticks += period_us;
      return true;
   }

   //! Idle until next tick event
   void yield()
   {
      while(!refresh())
      {
         sleepUS(period_us);
      }
   }

private:
   uint32_t period_us;
   uint32_t next_event_ticks;

   static uint32_t getTicksUS();
   static void     sleepUS(uint32_t);
};

} // namespace PLT

#endif // PLT_YIELD_H
