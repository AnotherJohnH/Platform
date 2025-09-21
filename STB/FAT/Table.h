//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace STB {

namespace FAT {

template <unsigned NUM_CLUSTERS>
class Table16
{
public:
   Table16()
   {
      entry[0] = 0xFFF8;
      entry[1] = 0xFFFF;
   }

   uint16_t operator[](unsigned index_) const { return entry[index_]; }

   void read(unsigned offset_, unsigned bytes_, uint8_t* buffer_) const
   {
      ::memcpy(buffer_, (const uint8_t*)entry + offset_, bytes_);
   }

   void write(unsigned offset_, unsigned bytes_, const uint8_t* buffer_) const
   {
      ::memcpy((uint8_t*)entry + offset_, buffer_, bytes_);
   }

   //! Allocate a chain of clusters returning the first
   uint16_t alloc(unsigned num_clusters_)
   {
      uint16_t first_cluster = getFreeCluster();
      uint16_t cluster       = first_cluster;

      for(; num_clusters_ > 1; --num_clusters_)
      {
         uint16_t next_cluster = getFreeCluster();
         entry[cluster] = next_cluster;
         cluster        = next_cluster;
      }

      entry[cluster] = 0xFFFF;

      return first_cluster;
   }

   const uint8_t* getPointer(unsigned offset_)
   {
      return ((const uint8_t*)entry) + offset_;
   }

private:
   uint16_t getFreeCluster()
   {
      // XXX Works until we can deallocate
      return free++;
   }

   uint16_t entry[NUM_CLUSTERS] = {0};
   uint32_t free{2};
};

} // namespace FAT

} // namespace STB
