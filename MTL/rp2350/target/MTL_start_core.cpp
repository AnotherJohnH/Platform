//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdint>
#include <cstdio>

#include "MTL/MTL.h"

#include "MTL/rp2350/Sio.h"

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
