//------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
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

// \file Keyboard.h
// \brief

#ifndef LPC1768_KEYBOARD_H
#define LPC1768_KEYBOARD_H


#include "MTL/chip/LPC1768/SSP.h"
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

#endif // LPC1768_KEYBOARD_H
