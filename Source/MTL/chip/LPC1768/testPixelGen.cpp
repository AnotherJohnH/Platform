//------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
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

// \brief 

#include "PixelGen.h"


void wait(volatile unsigned n)
{
   while(n--);
}


uint8_t buffer[40];


int main()
{
   MTL::PixelGen  pixel_gen;

   pixel_gen.resize(320, 256, 2);

   for(unsigned i=0; i<40; i++)
   {
      buffer[i] = 0xAA;
   }

   pixel_gen.setFramePtr(buffer);

   while(true)
   {
      pixel_gen.startField(0);

      pixel_gen.startLine(0);

      wait(1000000);

      pixel_gen.endLine();

      wait(1000);
   }
}
