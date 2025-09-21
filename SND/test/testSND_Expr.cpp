//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Test expression building

#include "PLT/Event.h"
#include "SND/SND.h"

int main(int argc, const char* argv[])
{
   // Construct components
   SND::SineOsc osc1{440.0};
   SND::SineOsc osc2{880.0};
   SND::Monitor monitor;
   SND::Add     add;
   SND::Mul     mul;

   monitor.in = mul(add(osc1, osc2), 0.5);

   monitor.play();

   return PLT::Event::mainLoop();
}
