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

#include <cstdint>
#include <cstdio>

#include "MTL/MTL.h"

#include "Sio.h"

extern uint32_t vector_table_core1;

static MTL::Sio sio;

static bool send(uint32_t data)
{
   sio.txFifoPush(data);
   __asm__("sev");

   uint32_t response = sio.rxFifoPop();

   return response == data;
}

bool MTL_start_core(unsigned core_num_, void (*func)())
{
   if (core_num_ != 1)
      return false;

   while(true)
   {
       sio.rxFifoDrain();
       __asm__("sev");
       if (not send(0)) continue;

       sio.rxFifoDrain();
       __asm__("sev");
       if (not send(0)) continue;

       if (not send(1)) continue;
       if (not send(uint32_t(&vector_table_core1))) continue;
       if (not send(uint32_t(vector_table_core1))) continue;
       if (not send(uint32_t(func) | 1)) continue;

       break;
   }

   return true;
}
