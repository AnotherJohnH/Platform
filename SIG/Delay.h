//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG.h"

namespace SIG {

class Delay
{
public:
   Delay() = default;

   Signal operator()() const { return prev; }

   void operator=(Signal x_) { prev = x_; }

   Signal operator()(Signal x_)
   {
      Signal y = prev;
      prev = x_;
      return y;
   }

private:
   Signal prev{0.0f};
};

template <unsigned LENGTH>
class DelayN
{
public:
   DelayN() = default;

   Signal operator()() const { return buffer[index]; }

   void operator=(Signal x_)
   {
      buffer[index] = x_;

      if (++index >= LENGTH)
         index = 0;
   }

   Signal operator()(Signal x_)
   {
      Signal y = buffer[index];

      operator=(x_);

      return y;
   }

private:
   unsigned index{0};
   Signal   buffer[LENGTH] = {};
};

} // namespace SIG
