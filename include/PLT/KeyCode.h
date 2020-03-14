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

#include <cstdint>

//! Platform abstraction layer
namespace PLT {

//! Event codes for special keys
enum KeyCode : uint8_t
{
   BACKSPACE = 0x08,
   TAB       = 0x09,
   RETURN    = 0x0A,
   ESCAPE    = 0x1B,

   DELETE    = 0x7F,
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
   BREAK     = 0x8F,
   UP        = 0x90,  //!< Cursor up
   DOWN      = 0x91,  //!< Cursor down
   LEFT      = 0x92,  //!< Cursor left
   RIGHT     = 0x93,  //!< Cursor right
   LSHIFT    = 0x94,  //!< Left shift
   RSHIFT    = 0x95,  //!< Right shift
   LCTRL     = 0x96,  //!< Left ctrl
   RCTRL     = 0x97,  //!< Right ctrl
   LALT      = 0x98,  //!< Left alt
   RALT      = 0x99,  //!< Right alt
   HOME      = 0x9A,
   END       = 0x9B,
   PAGE_UP   = 0x9C,
   PAGE_DOWN = 0x9D,
   SELECT    = 0x9E,
   MENU      = 0x9F,
   VOL_DOWN  = 0xA0,  //!< Volume down
   VOL_UP    = 0xA1,  //!< Volume up
   LCMD      = 0xA2,  //!< Left cmd
   RCMD      = 0xA3,  //!< Right cmd

   QUIT      = 0xF0,
   TIMEOUT   = 0xF1
};

//! Get key code for key when shift is pressed on a modern keyboard
inline uint8_t getShiftedKey(uint8_t base_key, bool ansi = true)
{
   if ((base_key >= 'a') && (base_key <= 'z'))
   {
      return base_key + 'A' - 'a';
   }

   switch(base_key)
   {
   case '`':  return ansi ? '~' : '\0';
   case '1':  return '!';
   case '2':  return ansi ? '@' : '"';
   case '3':  return ansi ? '#' : POUND;
   case '4':  return '$';
   case '5':  return '%';
   case '6':  return '^';
   case '7':  return '&';
   case '8':  return '*';
   case '9':  return '(';
   case '0':  return ')';
   case '-':  return '_';
   case '=':  return '+';

   case '[':  return '{';
   case ']':  return '}';
   case '\\': return '|';

   case ';':  return ':';
   case '\'': return ansi ? '"' : '@';
   case '#':  return '~';  // ISO only

   case ',':  return '<';
   case '.':  return '>';
   case '/':  return '?';
   }

   return base_key;
}

} // namespace PLT

#endif // PLT_KEY_CODE_H
