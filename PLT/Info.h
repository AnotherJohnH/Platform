//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file Info.h
//! \brief Platform abstraction layer for system information

#ifndef PLT_INFO_H
#define PLT_INFO_H

#include <cstdint>

//! Platform abstraction layer
namespace PLT {

//! Interface for system information
namespace Info {

//! Type of value
enum Value
{
   PWR_PERCENT,
   PWR_CHARGING,
   WIFI_ON,
   WIFI_STRENGTH
};

//! Get current value
//
//! \param type of value
//! \return value or -1 for no value
int32_t get(Value type);

} // namespace Info

} // namespace PLT

#endif // PLT_INFO_H
