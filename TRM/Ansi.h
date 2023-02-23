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

#pragma once

#include <cctype>
#include <cstdint>

namespace TRM {

//! State machine to help interpret ANSI/VT100 character streams
class Ansi
{
public:
   Ansi() = default;

   //! Supply next character to the state machine
   void ansiWrite(uint8_t ch)
   {
      if(ch == ESC)
      {
         escape = true;
      }
      else if(escape)
      {
         escape = false;

         if((ch >= 0x40) && (ch <= 0x5F))
         {
            ansiWrite(ch + 0x40);
         }
         else if(ch == 'c')
         {
            state       = State::BASE;
            string_size = 0;
            ansiReset();
         }
         else
         {
            // ignore
         }
      }
      else
      {
         switch(state)
         {
         case State::BASE:
            switch(ch)
            {
            case DCS: state = State::DEVICE_CONTROL_STRING;       break;
            case CSI: state = State::CONTROL_SEQUENCE_INTRODUCER; break;
            case OSC: state = State::OPERATING_SYSTEM_CONTROL;    break;
            case SOS: state = State::START_OF_STRING;             break;
            case PM:  state = State::PRIVACY_MESSAGE;             break;
            case APC: state = State::APPLICATION_PROGRAM_COMMAND; break;

            default:
               if ((ch < 0x20) || ((ch > 0x7E) && (ch < 0xA0)))
               {
                  ansiControl(ch);
               }
               else
               {
                  ansiGraphic(ch);
               }
               break;
            }
            break;

         case State::CONTROL_SEQUENCE_INTRODUCER:
            csiChar(ch);
            break;

         case State::DEVICE_CONTROL_STRING:
         case State::OPERATING_SYSTEM_CONTROL:
         case State::START_OF_STRING:
         case State::PRIVACY_MESSAGE:
         case State::APPLICATION_PROGRAM_COMMAND:
            stringChar(ch);
            break;
         }
      }
   }

   bool parseUInt(const char*& s, unsigned& value)
   {
      if(!isdigit(*s)) return false;

      value = 0;

      while(true)
      {
         unsigned digit = *s - '0';
         value          = (value * 10) + digit;
         ++s;
         if(!isdigit(*s)) break;
      }

      return true;
   }

protected:
   // Interface to be implemented by derived classes
   virtual void ansiReset() {}
   virtual void ansiControl(uint8_t ch) {}
   virtual void ansiGraphic(uint8_t ch) {}
   virtual void ansiCsi(uint8_t cmd, const char* seq) {}
   virtual void ansiDcs(const char* str) {}
   virtual void ansiOsc(const char* str) {}

private:
   //! Handle char in a CSI
   void csiChar(uint8_t ch)
   {
      if((ch >= 0x40) && (ch <= 0x7E))
      {
         string[string_size] = '\0';

         ansiCsi(ch, string);

         state       = State::BASE;
         string_size = 0;
      }
      else if(string_size < MAX_STRING_SIZE)
      {
         string[string_size++] = ch;
      }
   }

   //! Handle char in a string
   void stringChar(uint8_t ch)
   {
      if(ch == ST)
      {
         string[string_size] = '\0';

         switch(state)
         {
         case State::DEVICE_CONTROL_STRING:
            ansiDcs(string);
            break;

         case State::OPERATING_SYSTEM_CONTROL:
            ansiOsc(string);
            break;

         case State::START_OF_STRING:
         case State::PRIVACY_MESSAGE:
         case State::APPLICATION_PROGRAM_COMMAND:
            // Ignore
            break;

         default:
            break;
         }

         state       = State::BASE;
         string_size = 0;
      }
      else if(string_size < MAX_STRING_SIZE)
      {
         string[string_size++] = ch;
      }
   }

   static const size_t MAX_STRING_SIZE = 16;

   static const uint8_t ESC = 0x1B;
   static const uint8_t DCS = 0x90;
   static const uint8_t SOS = 0x98;
   static const uint8_t CSI = 0x9B;
   static const uint8_t ST  = 0x9C;
   static const uint8_t OSC = 0x9D;
   static const uint8_t PM  = 0x9E;
   static const uint8_t APC = 0x9F;

   enum class State
   {
      BASE,
      CONTROL_SEQUENCE_INTRODUCER,
      DEVICE_CONTROL_STRING,
      OPERATING_SYSTEM_CONTROL,
      START_OF_STRING,
      PRIVACY_MESSAGE,
      APPLICATION_PROGRAM_COMMAND
   };

   bool   escape{false};
   State  state{State::BASE};
   size_t string_size{0};
   char   string[MAX_STRING_SIZE + 1]{'\0'};
};

} // namespace TRM
