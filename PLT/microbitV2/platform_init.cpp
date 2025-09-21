//-------------------------------------------------------------------------------
// Copyright (c) 2020-2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/nRF52/Clock.h"
#include "MTL/nRF52/Nvmc.h"

extern "C" {

void MTL_init()
{
   MTL::nRF52::Clock().startHFXO();

   MTL::nRF52::Nvmc().enableICache();
}

}
