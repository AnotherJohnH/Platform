//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstddef>
#include <cstdint>
#include <array>

namespace STB {

template <size_t SIZE, typename WORD = uintptr_t>
class BitArray
{
public:
   class BitRef
   {
   public:
      BitRef(WORD mask_, WORD& word_)
         : mask(mask_)
         , word(word_)
      {
      }

      operator bool() const { return (word & mask) != 0; }

      bool operator=(bool value_)
      {
         if (value_)
            word |= mask;
         else
            word &= ~mask;

         return value_;
      }

   private:
      WORD  mask;
      WORD& word;
   };

   BitArray() = default;

   //! Number of bits
   size_t size() const { return SIZE; }

   //! Check if any bit is set
   bool any() const
   {
      for(const auto& word : data)
         if (word) return true;
      return false;
   }

   //! Check if all bits are set
   bool all() const
   {
      size_t i = 0;
      while(i < (NUM_WORDS - 1))
         if (data[i++] != ALL_MASK) return false;
      return data[i] == FINAL_MASK;
   }

   //! Check if no bits are set
   bool none() const
   {
      for(const auto& word : data)
         if (word != 0) return false;
      return true;
   }

   //! Return state of specific bit
   bool test(size_t pos_) const
   {
      size_t index;
      WORD   mask = getMask(pos_, index);
      return (data[index] & mask) != 0;
   }

   //! Set all bits true or given value
   void set()
   {
      size_t i = 0;
      while(i < (NUM_WORDS - 1))
         data[i++] = ALL_MASK;
      data[i] = FINAL_MASK;
   }

   //! Set all bits false
   void reset()
   {
      for(auto& word : data)
         word = 0;
   }

   //! Set a bit true or given value
   void set(size_t pos_)
   {
      operator[](pos_) = true;
   }

   //! Set a bit false
   void reset(size_t pos_)
   {
      operator[](pos_) = false;
   }

   //! Set a range of bits true or given value
   void set(size_t lo_, size_t hi_)
   {
      for(size_t pos = lo_; pos <= hi_; ++pos)
         set(pos);
   }

   //! Set a range of bits false
   void reset(size_t lo_, size_t hi_)
   {
      for(size_t pos = lo_; pos <= hi_; ++pos)
         reset(pos);
   }

   const BitRef operator[](size_t pos_) const
   {
      size_t index;
      return BitRef(getMask(pos_, index), data[index]);
   }

   BitRef operator[](size_t pos_)
   {
      size_t index{};
      return BitRef(getMask(pos_, index), data[index]);
   }

private:
   static WORD getMask(size_t pos_, size_t& index_)
   {
      index_       = pos_ / BITS_PER_WORD;
      unsigned bit = pos_ % BITS_PER_WORD;
      return WORD(1) << bit;
   }

   static const size_t BITS_PER_WORD = sizeof(WORD) * 8;
   static const size_t NUM_WORDS     = ((SIZE - 1) / BITS_PER_WORD) + 1;
   static const size_t FINAL_BITS    = SIZE % BITS_PER_WORD;
   static const WORD   ALL_MASK      = WORD(-1);
   static const WORD   FINAL_MASK    = (WORD(1) << FINAL_BITS) - 1;

   std::array<WORD,NUM_WORDS> data = {};
};

} // namespace STB
