//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
