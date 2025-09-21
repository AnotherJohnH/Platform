//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Record pin allocations

#pragma once

#include <cstring>
#include <cstdio>

namespace MTL {

template <unsigned PINS>
class ConfigBase
{
public:
   ConfigBase() = default;

   //! Declare pin usage
   void pin(unsigned pin_, const char* label_)
   {
      getRef(pin_) = label_;
   }

   //! Declare GPIO usage
   void gpio(unsigned gpio_, const char* label_)
   {
      pin(gpioToPin(gpio_), label_);
   }

   //! Format config into the gievn buffer
   size_t format(char* buffer_, size_t size_) const
   {
      unsigned left_max = 0;

      for(unsigned row = 1; row <= (NUM_PINS / 2); ++row)
      {
          unsigned left_pin = row;
          size_t len = strlen(get(left_pin));
          if (len > left_max)
             left_max = len;
      }

      char* s     = buffer_;
      char* end_s = buffer_ + size_;

      s += snprintf(s, end_s - s, "\n");

      for(unsigned row = 1; row <= (NUM_PINS / 2); ++row)
      {
          unsigned left_pin  = row;
          unsigned right_pin = NUM_PINS - row + 1;

          const char* label = get(left_pin);
          for(unsigned i = 0; i < (left_max - strlen(label)); ++i)
          {
             if (s < end_s)
                *s++ = ' ';
          }

          s += snprintf(s, end_s - s, "%s | %2u     %2u | %s\n",
                        label, left_pin, right_pin, get(right_pin));
      }

      return s - buffer_;
   }

protected:
   //! Convert GPIO number to PIN number
   virtual unsigned gpioToPin(unsigned gpio_)
   {
      return 0;
   }

   //! Get a reference to the label for the given pin
   const char*& getRef(unsigned pin_)
   {
      if ((pin_ < 1) || (pin_ > NUM_PINS))
         return label[0];

      return label[pin_];
   }

   //! Get the label for the given pin
   const char* get(unsigned pin_) const
   {
      if ((pin_ < 1) || (pin_ > NUM_PINS))
         return "";

      if (label[pin_] == nullptr)
         return "";

      return label[pin_];
   }

   static constexpr unsigned NUM_PINS = PINS;

private:
   const char* label[NUM_PINS];
};

} // namespace MTL
