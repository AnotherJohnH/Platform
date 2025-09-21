//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// The "Hello world!" of sound synthesis

#include <cstdio>

#include "PLT/Event.h"
#include "SND/SND.h"

int main(int argc, const char* argv[])
{
   // Construct components
   SND::SineOsc osc{440.0};
   SND::Monitor monitor;

   // Connect components
   monitor.in = osc;

   // Start audio sampling
   monitor.play();

   // Wait here for application exit
   PLT::Event::mainLoop();
}
