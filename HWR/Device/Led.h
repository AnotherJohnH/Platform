//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Hardware LED

#pragma once

#if defined(HW_LED)

#include "MTL/Digital.h"

#elif defined(HW_LED_NATIVE)

#include "native/Led.h"

#endif

namespace hw {

#if defined(HW_LED)

using Led = MTL::Digital::Out<HW_LED>;

#elif defined(HW_LED_NATIVE)

using Led = ::Led<>;

#else

using Led = bool;

#endif

} // namespace hw
