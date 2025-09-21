//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "PLT/Sounder.h"
#include "PLT/Event.h"


int main()
{
   PLT::Sounder sounder;

   sounder.setEnable(true);

   return PLT::Event::mainLoop();
}
