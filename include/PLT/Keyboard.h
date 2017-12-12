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

//! \file Keyboard.h
//! \brief 

#ifndef PLT_KEYBOARD_H
#define PLT_KEYBOARD_H

#include "PLT/Event.h"

#include <cstdlib>

//! Platform abstraction layer
namespace PLT {

class Keyboard
{
public:
   Keyboard() : shift(false) {}

   //! Get state of physical shift keys
   bool getShift() const { return shift; }

   //! Get key code for key when shift is pressed
   uint8_t getShiftedKey(uint8_t base_key) const
   {
      if (!shift) return base_key;

      switch(base_key)
      {
      case '1':  return '!';
      case '2':  return '@';
      case '3':  return POUND;
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

      case ';':  return ':';
      case '\'': return '"';
      case '\\': return '|';

      case '`':  return '~';
      case ',':  return '<';
      case '.':  return '>';
      case '/':  return '?';

      default:   return ((base_key >= 'a') && (base_key <= 'z')) ? base_key + 'A' - 'a'
                                                                 : base_key;
      }
   }

   //! Poll for next keyboard event
   bool poll(uint8_t& key, bool& down)
   {
      Event  event;

      switch(pollEvent(event))
      {
      case KEY_DOWN:
         key  = event.code;
         down = true;
         if ((key == KeyCode::LSHIFT) || (key == KeyCode::RSHIFT))
         {
            shift = true;
         }
         return true;

      case KEY_UP:
         key  = event.code;
         down = false;
         if ((key == KeyCode::LSHIFT) || (key == KeyCode::RSHIFT))
         {
            shift = false;
         }
         return true;

      case QUIT:
         exit(0);
         break;

      default:
         return false;
      }
   }

private:
   bool  shift;
};


} // namespace PLT

#endif // PLT_KEYBOARD_H
