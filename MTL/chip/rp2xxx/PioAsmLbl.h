//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

// \brief Code label class for use with PioAsm

#pragma once

#include <cassert>
#include <cstdint>

namespace PIO {

class Lbl
{
public:
   Lbl() = default;

   Lbl(uint8_t addr5_)
      : resolved(true)
      , addr5(addr5_)
   {
   }

   ~Lbl()
   {
      assert(unresolved == 0);
   }

   operator uint8_t() const
   {
      assert(resolved);
      return addr5;
   }

   //! Reference a label
   uint8_t reference(uint8_t addr5_)
   {
       if (not resolved)
       {
          unresolved |= 1 << addr5_;
       }

       return addr5;
   }

   //! Resolve a label
   void resolve(uint8_t addr5_, uint16_t* prog_)
   {
      resolved = true;
      addr5    = addr5_;

      for(unsigned a = 0; a < 32; a++)
      {
         if (unresolved & (1 << a))
         {
            prog_[a] |= addr5_;
         }
      }

      unresolved = 0;
   }

private:
   bool     resolved{false};
   uint8_t  addr5{0};
   uint32_t unresolved{0};
};

} // namespace PIO
