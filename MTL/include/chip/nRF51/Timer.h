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
// Timer peripheral
//

#ifndef MTL_NRF51_TIMER_H
#define MTL_NRF51_TIMER_H

#include "MTL/Periph.h"

#include "MTL/core/CortexM0/NVIC.h"

#include "Irq.h"

namespace MTL {

namespace nRF51 {

enum TimerClock
{
   TIMER_16_MHZ    = 0,
   TIMER_8_MHZ     = 1,
   TIMER_4_MHZ     = 2,
   TIMER_2_MHZ     = 3,
   TIMER_1_MHZ     = 4,
   TIMER_500_KHZ   = 5,
   TIMER_250_KHZ   = 6,
   TIMER_125_KHZ   = 7,
   TIMER_62_5_KHZ  = 8,
   TIMER_31_25_KHZ = 9
};

enum TimerWidth
{
   TIMER_WIDTH_8_BITS  = 1,
   TIMER_WIDTH_16_BITS = 0,
   TIMER_WIDTH_24_BITS = 2,
   TIMER_WIDTH_32_BITS = 3
};

enum TimerAction
{
   TIMER_NONE  = 0,
   TIMER_CLEAR = 1<<0,
   TIMER_STOP  = 1<<1,
   TIMER_IRQ   = 1<<2
};

union TimerReg
{
   REG(0x000, task_start);
   REG(0x004, task_stop);
   REG(0x008, task_count);
   REG(0x00C, task_clear);
   REG(0x010, task_shutdown);

   REG_ARRAY(0x040, task_capture,  4);
   REG_ARRAY(0x140, event_compare, 4);

   REG(0x200,  shorts);
   REG(0x304,  intenset);
   REG(0x308,  intenclr);
   REG(0x504,  mode);
   REG(0x508,  bitmode);
   REG(0x510,  prescaler);

   REG_ARRAY(0x540, cc, 4);
};


template<unsigned INDEX>
class Timer : public Periph<TimerReg,0x40008000,INDEX>
{
private:
   static const unsigned F_CLK = 16000000;

   NVIC<IRQ_TIMER_0+INDEX> irq;

   using Periph<TimerReg,0x40008000,INDEX>::reg;

public:
   Timer(TimerWidth width = TIMER_WIDTH_16_BITS)
   {
      stop();

      reg->bitmode = width;

      // Configure 1MHz timer by default
      setModeIsTimer();
      setClock(TIMER_1_MHZ);

      irq.enable();
   }

   void setModeIsCounter() { reg->mode = 1; }

   void setModeIsTimer()   { reg->mode = 0; }

   void setClock(TimerClock log2_divisor)
   {
      stop();
      reg->prescaler = log2_divisor;
   }

   //! Width of counter register (bits)
   unsigned getWidth() const
   {
      switch(reg->bitmode)
      {
      case TIMER_WIDTH_8_BITS:  return 8;
      case TIMER_WIDTH_16_BITS: return 16;
      case TIMER_WIDTH_24_BITS: return 24;
      case TIMER_WIDTH_32_BITS: default: return 32;
      }
   }

   //! Maximum count value
   uint32_t getMaxCount() const
   {
      return (1<<getWidth()) - 1;
   }

   bool isCompareEvent(unsigned index) const
   {
      return reg->event_compare[index];
   }

   void clrCompareEvent(unsigned index)
   {
      reg->event_compare[index] = 0;
   }

   void setCompare(unsigned index, uint32_t ticks)
   {
      reg->cc[index] = ticks;
   }

   void setAction(unsigned index, uint32_t ticks,
                  uint32_t action = TIMER_NONE)
   {
      setCompare(index, ticks);

      reg->shorts.setBit(index,   (action & TIMER_CLEAR) != 0);

      reg->shorts.setBit(index+8, (action & TIMER_STOP)  != 0);

      if (action & TIMER_IRQ)
         reg->intenset.setBit(index + 16);
      else
         reg->intenclr.setBit(index + 16);
   }

   void setFreqHz(unsigned index, unsigned freq,
                  uint32_t action = TIMER_NONE)
   {
      stop();

      if (freq != 0)
      {
         unsigned  prescaler;
         uint32_t  ticks = 0;
         unsigned  width = getWidth();

         for(prescaler=0; prescaler<=9; prescaler++)
         {
            unsigned f_timer  = F_CLK/(1<<prescaler);
            unsigned min_freq = f_timer >> width;

            if (freq > min_freq)
            {
               ticks = f_timer / freq;
               break;
            }
         }

         setClock(TimerClock(prescaler));
         setAction(index, ticks, action | TIMER_CLEAR);
      }
   }

   void setPeriodUs(unsigned index, unsigned period,
                    uint32_t action = TIMER_NONE)
   {
      setFreqHz(index, 1000000/period, action);
   }

   void setIrqUs(unsigned index, unsigned period)
   {
      unsigned log2_divisor = reg->prescaler;
      unsigned f_timer      = F_CLK/(1<<log2_divisor);
      unsigned ticks        = period*f_timer/1000000;

      setAction(index, ticks, TIMER_IRQ);
   }

   // Tasks
   void start()    { reg->task_start    = 1; }
   void stop()     { reg->task_stop     = 1; }
   void count()    { reg->task_count    = 1; }
   void clear()    { reg->task_clear    = 1; }
   void shutdown() { reg->task_shutdown = 1; }

   void capture(unsigned index)
   {
      reg->task_capture[index] = 1;
   }

   uint32_t getCount(unsigned index)
   {
      capture(index);
      return reg->cc[index];
   }

   uint32_t getEvent(unsigned index)
   {
      return uint32_t(&reg->event_compare[index]);
   }
};

} // namespace nRF51

} // namespace MTL

#endif
