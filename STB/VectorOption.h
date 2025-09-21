//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <vector>

#include "STB/Option.h"

namespace STB {

template <typename TYPE>
class VectorOption : public Option<TYPE>
{
public:
   VectorOption(char short_opt, const char* long_opt, const char* description)
      : Option<TYPE>(short_opt, long_opt, description)
   {}
   
   const std::vector<TYPE>& list() const { return value_list; }

private:
   bool set(const char* value) override
   {  
      bool ok = STB::Option<TYPE>::set(value);
      if (ok)
      {  
         value_list.push_back(this->get());
      }
      return ok;
   }
   
   std::vector<TYPE> value_list;
};

} // namespace STB

