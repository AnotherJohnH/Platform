//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MTL/LPC1768/SSP.h"
#include "MTL/PS2KeyDecode.h"

namespace MTL {


class Keyboard
{
private:
   SSP<0>         ssp;
   PS2KeyDecode   ps2;

public:
   Keyboard() : ssp(11, /* master */ false)
   {
      ssp.enablePin_SCK();
      ssp.enablePin_MOSI();
   }

   bool recv(uint8_t& ch, bool& is_up)
   {
      while(!ssp.isRxFifoEmpty())
      {
         // Remove stop and parity bits
         uint8_t byte = uint8_t(ssp.recv() >> 1);

         // Reverse remaining bits
         byte = ((byte & 0xAA) >> 1) | ((byte & 0x55) << 1);
         byte = ((byte & 0xCC) >> 2) | ((byte & 0x33) << 2);
         byte = ((byte & 0xF0) >> 4) | ((byte & 0x0F) << 4);

         if (ps2.decode(byte, ch, is_up))
         {
            return true;
         }
      }

      return false;
   }
};

} // namespace MTL
