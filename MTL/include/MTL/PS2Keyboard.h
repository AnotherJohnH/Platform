//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MTL/PS2KeyDecode.h"

namespace MTL {

template <class SERIAL>
class PS2Keyboard
{
private:
   SERIAL       serial;
   PS2KeyDecode ps2;

public:
   // TODO baud rate should be Hz not nRF51 specific value
   PS2Keyboard()
      : serial(0x358000, /* parity */ true) {}   // 13 KHz

   bool recv(uint8_t& ch, bool& is_up)
   {
      while(!serial.isRxFifoEmpty())
      {
         if (ps2.decode(serial.recv(), ch, is_up))
         {
            return true;
         }
      }

      return false;
   }
};

} // namespace MTL
