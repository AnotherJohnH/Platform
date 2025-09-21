//-------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Declare Cortex-M33 vector entry points

#pragma once

extern "C"
{
   void VEC_nmi();
   void VEC_fault();
   void VEC_svc();
   void VEC_pendSv();
   void VEC_sysTick();
}
