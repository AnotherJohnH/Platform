//------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#ifndef STB_VECTOR_OPTION_H
#define STB_VECTOR_OPTION_H

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

#endif
