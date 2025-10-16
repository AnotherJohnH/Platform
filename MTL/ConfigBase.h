//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Record pin allocations

#pragma once

#include <cstdio>
#include <cstring>

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
         unsigned    left_pin = row;
         const char* dir;
         size_t      len = strlen(get(left_pin, /* left */ true, dir));
         if (len > left_max)
            left_max = len;
      }

      char* s     = buffer_;
      char* end_s = buffer_ + size_;

      s += snprintf(s, end_s - s, "\n");

      writeSpace(s, end_s, left_max);
      s += snprintf(s, end_s - s, "    +-----------+\n");

      for(unsigned row = 1; row <= (NUM_PINS / 2); ++row)
      {
         unsigned    lft_pin = row;
         unsigned    rgt_pin = NUM_PINS - row + 1;
         const char* lft_dir;
         const char* rgt_dir;
         const char* lft_label = get(lft_pin, /* left */ true,  lft_dir);
         const char* rgt_label = get(rgt_pin, /* left */ false, rgt_dir);

         writeSpace(s, end_s, left_max - strlen(lft_label));

         s += snprintf(s, end_s - s, "%s %s | %2u     %2u | %s %s\n",
                       lft_label, lft_dir, lft_pin, rgt_pin, rgt_dir, rgt_label);
      }

      writeSpace(s, end_s, left_max);
      s += snprintf(s, end_s - s, "    +-----------+\n");

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
   const char* get(unsigned pin_, bool left_is_out_, const char*& dir_) const
   {
      dir_ = "  ";

      if ((pin_ < 1) || (pin_ > NUM_PINS))
         return "";

      const char* lbl = label[pin_];

      if (lbl == nullptr)
         return "";

      switch(lbl[0])
      {
      case '<': lbl++; dir_ = left_is_out_ ? "->" : "<-"; break;
      case '>': lbl++; dir_ = left_is_out_ ? "<-" : "->"; break;
      case '=': lbl++; dir_ = "<>"; break;
      default: break;
      }
 
      return lbl;
   }

   static constexpr unsigned NUM_PINS = PINS;

private:
   static void writeSpace(char*& s_, const char* end_s_, unsigned n_)
   {
      for(unsigned i = 0; i < n_; ++i)
      {
         if (s_ >= end_s_)
            break;

         *s_++ = ' ';
      }
   }

   const char* label[NUM_PINS];
};

} // namespace MTL
