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

#ifndef PLT_KEY_CODE_H
#define PLT_KEY_CODE_H

#include <stdint.h>

namespace PLT {

//! User interface key codes
enum KeyCode : uint8_t
{
   BACKSPACE = 0x08,
   TAB       = 0x09,
   RETURN    = 0x0A,
   ESCAPE    = 0x1B,

   POUND     = 0x80,
   F1        = 0x81,
   F2        = 0x82,
   F3        = 0x83,
   F4        = 0x84,
   F5        = 0x85,
   F6        = 0x86,
   F7        = 0x87,
   F8        = 0x88,
   F9        = 0x89,
   F10       = 0x8A,
   F11       = 0x8B,
   F12       = 0x8C,

   CAPSLOCK  = 0x8E,
   BACK      = 0x8F,
   UP        = 0x90,
   DOWN      = 0x91,
   LEFT      = 0x92,
   RIGHT     = 0x93,
   LSHIFT    = 0x94,
   RSHIFT    = 0x95,
   LCTRL     = 0x96,
   LALT      = 0x98,
   RALT      = 0x99,
   HOME      = 0x9A,
   END       = 0x9B,
   PAGE_UP   = 0x9C,
   PAGE_DOWN = 0x9D,
   SELECT    = 0x9E,
   MENU      = 0x9F,
   VOL_DOWN  = 0xA0,
   VOL_UP    = 0xA1
};

} // PLT

#endif