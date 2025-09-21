//-------------------------------------------------------------------------------
// Copyright (c) 2014-2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file Sounder.h
//! \brief Platform abstraction interface for simple "beep" audio hardware

#pragma once

//! Platform abstraction layer
namespace PLT {

//! Simple on/off sounder
class Sounder
{
public:
   //! Enable or disable the sounder
   //
   //! \param enable true => enable, false => disable
   void setEnable(bool enable);

   //! Fine control of the sounder
   //
   //! \param vol_8 gain 0..255
   //! \param freq  frequency (Hz)
   void control(unsigned vol_8, unsigned freq);
};

} // namespace PLT

