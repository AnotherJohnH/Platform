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

#ifndef MTL_PS2_KEYBOARD_H
#define MTL_PS2_KEYBOARD_H

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

#endif
