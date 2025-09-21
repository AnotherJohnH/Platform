//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef STB_DEFLATE_H
#define STB_DEFLATE_H

#include <cstdint>
#include <string>

namespace STB {

class Deflate
{
public:
   class Io
   {
   public:
      virtual uint8_t getByte()        = 0;
      virtual void    putByte(uint8_t) = 0;
      virtual void    error(const std::string&) {}
   };

   Deflate(Io* io_) : io(io_) {}

   //! Inflate DEFLATE stream
   size_t inflate(size_t window_size);

private:
   Io* io{nullptr};
};

} // namespace STB

#endif
