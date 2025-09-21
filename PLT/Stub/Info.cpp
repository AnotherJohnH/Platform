//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Stub Info implementation

#include "PLT/Info.h"

namespace PLT {

namespace Info {

int32_t get(Value type)
{
   switch(type)
   {
   case PWR_PERCENT: break;
   case PWR_CHARGING: break;
   case WIFI_ON: break;
   case WIFI_STRENGTH: break;
   }

   return -1;
}

} // namespace Info

} // namespace PLT
