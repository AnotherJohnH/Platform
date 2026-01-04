//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG.h"

namespace SIG {

template <unsigned LENGTH>
class Delay
{
public:
   Delay() = default;

   Signal operator()() const
   {
      return buffer[index];
   }

   void operator=(Signal x_)
   {
      buffer[index] = x_;

      if (++index > LENGTH)
         index = 0;
   }

   Signal operator()(Signal x_)
   {
      Signal y = buffer[index];

      operator=(x_);

      return y;
   }

private:
   unsigned index{};
   Signal   buffer[LENGTH];
};

} // namespace SIG
