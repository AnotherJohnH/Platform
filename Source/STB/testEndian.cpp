//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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


#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "STB/Endian.h"


template <typename T, typename U> void checkEQ(T value, U exp, const char* file, unsigned line)
{
   if(value != T(exp))
   {
      fprintf(stderr, "FAILED: %s:%u expected %u got %u\n", file, line, exp, value);
      abort();
   }
}


#define EQ(A, B) checkEQ(A, B, __FILE__, __LINE__)


void testBig32()
{
   STB::Big32     big32;
   const uint8_t* byte = (const uint8_t*)&big32;

   big32 = 0x12345678;

   EQ(byte[0], 0x12);
   EQ(byte[1], 0x34);
   EQ(byte[2], 0x56);
   EQ(byte[3], 0x78);

   big32 = STB::endianSwap(big32);

   EQ(byte[0], 0x78);
   EQ(byte[1], 0x56);
   EQ(byte[2], 0x34);
   EQ(byte[3], 0x12);
}


void testLtl32()
{
   STB::Ltl32     ltl32;
   const uint8_t* byte = (const uint8_t*)&ltl32;

   ltl32 = 0x12345678;

   EQ(byte[0], 0x78);
   EQ(byte[1], 0x56);
   EQ(byte[2], 0x34);
   EQ(byte[3], 0x12);

   ltl32 = STB::endianSwap(ltl32);

   EQ(byte[0], 0x12);
   EQ(byte[1], 0x34);
   EQ(byte[2], 0x56);
   EQ(byte[3], 0x78);
}


int main(int argc, const char* argv[])
{
   testBig32();
   testLtl32();

   printf("OK\n");

   return 0;
}
