//------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
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

//! \file PS2KeyDecode.h
//! \brief 

#ifndef MTL_PS2KEYDECODE_H
#define MTL_PS2KEYDECODE_H

#include <stdint.h>

namespace MTL {

class PS2KeyDecode
{
private:
   bool       extend;
   bool       up;
   uint8_t    shift;

   static const uint8_t scan_table[256];

public:
   PS2KeyDecode() : extend(false), up(false), shift(0) {}

   bool decode(uint8_t code, uint8_t& ch, bool& is_up)
   {
      switch(code)
      {
      case 0xE0: extend = true; return false;
      case 0xF0: up     = true; return false;;

      default:
         if (code == 0x12)
         {
            shift = up ? 0x00 : 0x80;
         }
         ch     = scan_table[code | shift];
         is_up  = up;
         extend = false;
         up     = false;
         return true;
      }
   }
};

} // namespace MTL

#endif // MTL_PS2KEYDECODE_H
