//------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
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

// \brief Use the PWM to generate a PAL horizontal sync pulse


#include "MTL/chip/LPC1768/PWM.h"


static const unsigned H_LINE_SYNC   = 4700 * 96 / 1000;
static const unsigned H_LINE_PERIOD = 64 * 96;


int main()
{
   // Uses PIN_2_5 which is PIN_DIP21
   MTL::PWM  pwm(1);

   // 64us sync with a 4.7uS sync pulse
   pwm.setRise(H_LINE_SYNC);
   pwm.setFall(1);
   pwm.setPeriod(H_LINE_PERIOD);

   pwm.start();

   //pwm.setIRQ<1>(H_IMAGE);
   //pwm.setIRQ<0>(H_LINE_PERIOD);
}
