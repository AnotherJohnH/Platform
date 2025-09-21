//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Declare Cortex-M4 vector entry points

#pragma once

extern "C"
{
   void VEC_nmi();
   void VEC_hard_fault();
   void VEC_mem_fault();
   void VEC_bus_fault();
   void VEC_usage_fault();
   void VEC_svc();
   void VEC_pendSv();
   void VEC_sysTick();
}
