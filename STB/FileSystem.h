//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace STB {

class FileSystem
{
public:
   FileSystem() = default;

   virtual unsigned getBlockSize() const = 0;

   virtual unsigned getNumBlocks() const = 0;

   virtual void read(uint32_t block_address_,
                     unsigned block_offset_,
                     unsigned bytes_,
                     uint8_t* buffer_) = 0;

   virtual void write(uint32_t       block_address_,
                      unsigned       block_offset_,
                      unsigned       bytes_,
                      const uint8_t* buffer_) = 0;

   virtual void endOfWrite() {}
   virtual void endOfRead() {}
};

} // namespace STB
