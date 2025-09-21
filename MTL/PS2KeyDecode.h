//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Convert PS2 scan codes to ASCII

#pragma once

#include <cstdint>

//! Bare metal layer
namespace MTL {

//! Convert PS2 scan codes to ASCII
class PS2KeyDecode
{
public:
   PS2KeyDecode() = default;

   //! Decode next scan code byte \return true if ASCII code bte available
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

private:
   bool    extend{false};
   bool    up{false};
   uint8_t shift{0};

   static const uint8_t scan_table[256];
};

} // namespace MTL
