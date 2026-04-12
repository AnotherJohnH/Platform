//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

//! Round to 2 decimal places
template <typename TYPE>
constexpr TYPE round2(TYPE value_)
{
   return TYPE(int(value_ * TYPE{100.0} + TYPE{0.5})) / TYPE{100.0};
}

//! Round to 3 decimal places
template <typename TYPE>
constexpr TYPE round3(TYPE value_)
{
   return TYPE(int(value_ * TYPE{1000.0} + TYPE{0.5})) / TYPE{1000.0};
}
