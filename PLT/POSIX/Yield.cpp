//------------------------------------------------------------------------------
// Copyright (c) 2017-2021 John D. Haughton
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

// \brief Stub PLT::Yield implementation 

#include <sys/time.h>
#include <unistd.h>

#include "PLT/Yield.h"

const int32_t PLT::Yield::TICKS_PER_SEC = 1000000;

int32_t PLT::Yield::getTicks()
{
   struct timeval t;

   gettimeofday(&t, 0);

   return t.tv_sec * 1000000 + t.tv_usec;
}

void PLT::Yield::yieldForTicks(int32_t period_ticks)
{
   usleep(period_ticks);
}

