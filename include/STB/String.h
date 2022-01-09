//------------------------------------------------------------------------------
// Copyright (c) 2022 John D. Haughton
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

#include <cstring>
#include <cstdint>

namespace STB {

class String
{
public:
   String(char* data_, uint32_t size)
      : data(data_)
      , max(size - 1)
   {
      clear();
   }

   //! Get the value of the string
   operator const char*() const { return data; }

   //! Number of characters in the string
   uint32_t size() const { return n; }

   //! Number of characters that can be stored
   uint32_t capacity() const { return max; }

   //! Check if the string is empty
   bool empty() const { return size() == 0; }

   //! Check if the string is full
   bool full() const { return size() == capacity(); }

   //! Check if this string is the same as another
   bool operator==(const char* that) const { return strcmp(data, that) == 0; }

   //! Check if this string is not the same as another
   bool operator!=(const char* that) const { return not this->operator==(that); }

   //! Make the string empty
   void clear()
   {
      n = 0;
      data[0] = '\0';
   }

   //! Append a character to the end of the string
   void push_back(char ch)
   {
      if (full()) return;

      data[n++] = ch;
      data[n]   = '\0';
   }

   //! Append a character to the end of the string
   void append(const char* s)
   {
      while(*s != '\0')
      {
         push_back(*s++);
      }
   }

   //! Append a character to the end of the string
   void operator+=(char ch) { push_back(ch); }

   //! Append a string to the end of the string
   void operator+=(const char* s) { append(s); }

   //! Assign a string
   void operator=(const char* s) { clear(); append(s); }

   //! Append a hexadecimal number to a C string
   template <typename TYPE>
   void bin(TYPE value, unsigned width = sizeof(TYPE) * 8, char pad = '0')
   {
      formatPowerOf2Base(value, 1, width, pad);
   }

   //! Append an octal number to the string
   template <typename TYPE>
   void oct(TYPE value, unsigned width = sizeof(TYPE) * 8 / 3, char pad = '0')
   {
      formatPowerOf2Base(value, 3, width, pad);
   }

   //! Append a hexadecimal number to the string
   template <typename TYPE>
   void hex(TYPE value, unsigned width = sizeof(TYPE) * 2, char pad = '0')
   {
      formatPowerOf2Base(value, 4, width, pad);
   }

   //! Append a decimal number to the string
   template <typename TYPE>
   void dec(TYPE value, unsigned width = 0, char pad = ' ')
   {
      unsigned pos{};
      TYPE     place{};

      switch(sizeof(TYPE))
      {
      case 1: pos =  3; place = TYPE(100);                 break;
      case 2: pos =  5; place = TYPE(10000);               break;
      case 4: pos = 10; place = TYPE(1000000000);          break;
      case 8: pos = 19; place = TYPE(1000000000000000000); break;
      default: break;
      }

      bool leading_zero = true;

      while(place != 0)
      {
         unsigned digit = value / place;
         value -= digit * place;
         place = place / 10;

         if (digit == 0)
         {
            if (leading_zero && (pos != 1))
            {
               if (pos <= width)
               {
                  push_back(pad);
               }

               pos--;
            }
            else
            {
               push_back('0');
            }
         }
         else
         {
            leading_zero = false;
            push_back(digit + '0');
         }
      }
   }

   //! Append a formatted string
   void str(const char* s, unsigned width = 0, bool left = true)
   {
      if (width == 0)
      {
         append(s);
         return;
      }

      size_t len = strlen(s);
      if (len >= width)
      {
         append(s);
         return;
      }

      for(unsigned i = 0; i < width; ++i)
      {
         bool space = left ? i >= len
                           : i < (width - len);
         if (space)
            push_back(' ');
         else
            push_back(*s++);
      }
   }

   //! Pad to the given position in the string
   void pad(unsigned col, char ch = ' ')
   {
      while(size() < col)
      {
         push_back(ch);
      }
   }

private:
   //! Append a hexadecimal number to a C string
   template <typename TYPE>
   void formatPowerOf2Base(TYPE     value,
                           unsigned log2_base,
                           unsigned width,
                           char     pad)
   {
      unsigned num_bits   = sizeof(TYPE) * 8;
      unsigned max_digits = num_bits / log2_base;
      unsigned base       = 1 << log2_base;
      TYPE     mask       = base - 1;

      bool leading_zero = true;

      for(unsigned i = 0; i < max_digits; ++i)
      {
         unsigned pos   = max_digits - i;
         unsigned lsb   = (pos - 1) * log2_base;
         unsigned digit = (value >> lsb) & mask;

         if (digit == 0)
         {
            if (leading_zero && (pos != 1))
            {
               if (pos <= width)
               {
                  push_back(pad);
               }

               pos--;
            }
            else
            {
               push_back('0');
            }
         }
         else
         {
            leading_zero = false;
            push_back(digit < 10 ? digit + '0' : digit - 10 + 'A');
         }
      }
   }

   char* const    data{};
   const uint32_t max{};
   uint32_t       n{};
};


template <uint32_t SIZE>
class StringArray : public String
{
public:
   StringArray()
      : String(buffer, SIZE)
   {
   }

private:
   char buffer[SIZE];
};

} // namespace
