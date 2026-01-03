//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Temperature sensor interface

#pragma once

#if defined(HW_TEMP_SENSE_P21_P22_MCP9808)

#include "MTL/TempSens_MCP9808.h"
#include "MTL/rp2040/I2C.h"

#elif defined(HW_TEMP_SENSE_BADGER_MCP9808)

#include "MTL/TempSens_MCP9808.h"

#elif defined(HW_TEMP_SENSE_TUFTY_MCP9808)

#include "MTL/TempSens_MCP9808.h"

#elif defined(HW_TEMP_SENSE_NATIVE)

#include "native/TempSense.h"

#endif

namespace hw {

#if defined(HW_TEMP_SENSE_P21_P22_MCP9808)

class TempSense : public MTL::TempSens_MCP9808<MTL::I2C0_P21_P22>
{
public:
   TempSense()
   {
      MTL::config.gpio(MTL::I2C0_P21_P22::SDA, "=I2C SDA MCP9808 (temp sense)");
      MTL::config.gpio(MTL::I2C0_P21_P22::SCL, ">I2C SCL MCP9808 (temp sense)");
   }
};

#elif defined(HW_TEMP_SENSE_BADGER_MCP9808)

class TempSense : public MTL::TempSens_MCP9808<MTL::badger2040::I2C_QwSt>
{
public:
   TempSense()
   {
      MTL::config.gpio(MTL::badger2040::I2C_QwSt::SDA, "=(temp) I2C SDA MCP9808");
      MTL::config.gpio(MTL::badger2040::I2C_QwSt::SCL, ">(temp) I2C SCL MCP9808");
   }
};

#elif defined(HW_TEMP_SENSE_TUFTY_MCP9808)

class TempSense : public MTL::TempSens_MCP9808<MTL::tufty2040::I2C_QwSt>
{
public:
   TempSense()
   {
      MTL::config.gpio(MTL::tufty2040::I2C_QwSt::SDA, "=(temp) I2C SDA MCP9808");
      MTL::config.gpio(MTL::tufty2040::I2C_QwSt::SCL, ">(temp) I2C SCL MCP9808");
   }
};

#elif defined(HW_TEMP_SENSE_NATIVE)

class TempSense : public ::TempSens
{
public:
   TempSense() = default;
};

#endif

} // namespace hw
