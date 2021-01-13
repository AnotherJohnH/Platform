//------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
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
// PWM peripheral
//

#ifndef MTL_NRF52_PWM_H
#define MTL_NRF52_PWM_H

#include "MTL/Periph.h"

#include "MTL/core/CortexM3/NVIC.h"

#include "Gpio.h"
#include "Irq.h"

namespace MTL {

namespace nRF52 {

union PwmReg
{
   REG(0x004, task_stop);
   REG(0x008, task_seq0_start);
   REG(0x00C, task_seq1_start);
   REG(0x010, task_nextstep);
   REG(0x104, events_stopped);
   REG(0x108, events_seqstart0);
   REG(0x10C, events_seqstart1);
   REG(0x114, events_seqend);
   REG(0x118, events_pwmperiodend);
   REG(0x11C, events_loopsdone);
   REG(0x200, shorts);
   REG(0x300, inten);
   REG(0x304, intenset);
   REG(0x308, intenclr);
   REG(0x500, enable);
   REG(0x504, mode);
   REG(0x508, countertop);
   REG(0x50C, prescaler);
   REG(0x510, decoder);
   REG(0x514, loop);
   REG(0x520, seq0_ptr);
   REG(0x524, seq0_cnt);
   REG(0x528, seq0_refresh);
   REG(0x52C, seq0_enddelay);
   REG(0x540, seq1_ptr);
   REG(0x544, seq1_cnt);
   REG(0x548, seq1_refresh);
   REG(0x54C, seq1_enddelay);

   REG_ARRAY(0x560, pselout, 4);
};

template<uint32_t BASE, unsigned IRQ, unsigned PIN>
class PwmInstance : public Periph<PwmReg,BASE>
{
public:
   static unsigned getClock() { return 16000000; }

   PwmInstance(unsigned log2_divisor_7 = 0)
   {
      reg->enable        = 1;
      reg->mode          = 0;           // Count up
      reg->pselout[0]    = PIN;
      reg->prescaler     = log2_divisor_7;
      reg->loop          = 0;
      reg->decoder       = 0;
      reg->seq0_ptr      = uint32_t(&seq0);
      reg->seq0_cnt      = 1;
      reg->seq0_refresh  = 0;
      reg->seq0_enddelay = 0;
   }

   void setClkDiv(unsigned divisor_7)
   {
      reg->prescaler = divisor_7;
   }

   void program(unsigned mark_15, unsigned period_15)
   {
      seq0            = mark_15;
      reg->countertop = period_15;
   }

   void start()
   {
      reg->task_seq0_start = 1;
   }

   void stop()
   {
      reg->task_stop = 1;
   }

private:
   Gpio::Out<1,PIN> gpio;
   NVIC<IRQ>        irq;

   using Periph<PwmReg,BASE>::reg;

   uint16_t seq0;
};

template<unsigned PIN>
using Pwm0 = PwmInstance<0x4001C000,IRQ_PWM_0,PIN>;

template<unsigned PIN>
using Pwm1 = PwmInstance<0x40021000,IRQ_PWM_1,PIN>;

template<unsigned PIN>
using Pwm2 = PwmInstance<0x40022000,IRQ_PWM_2,PIN>;

template<unsigned PIN>
using Pwm3 = PwmInstance<0x4002D000,IRQ_PWM_3,PIN>;

} // namespace nRF52

} // namespace MTL

#endif
