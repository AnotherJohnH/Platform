//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace PHY {

struct Hydrogen
{
   static const char* symbol() { return "H"; };
   static const unsigned   Z = 1;     // Atomic number, [protons]
   static const unsigned   A = 1;     // Total number of protons and neutrons
   static constexpr double M = 1.0;   // Molar mass [g/mol]
};

struct Helium
{
   static const char* symbol() { return "He"; };
   static const unsigned   Z = 2;     // Atomic number, i.e. number of protons
   static const unsigned   A = 4;     // Total number of protons and neutrons
   static constexpr double M = 4.0;   // Molar mass [g/mol]
};

struct Carbon
{
   static const char* symbol() { return "C"; };
   static const unsigned   Z = 6;     // Atomic number, i.e. number of protons
   static const unsigned   A = 12;    // Total number of protons and neutrons
   static constexpr double M = 12.0;  // Molar mass [g/mol]
};

struct Nitrogen
{
   static const char* symbol() { return "N"; };
   static const unsigned   Z = 7;     // Atomic number, i.e. number of protons
   static const unsigned   A = 14;    // Total number of protons and neutrons
   static constexpr double M = 14.0;  // Molar mass [g/mol]
};

struct Oxygen
{
   static const char* symbol() { return "O"; };
   static const unsigned   Z = 8;     // Atomic number, i.e. number of protons
   static const unsigned   A = 16;    // Total number of protons and neutrons
   static constexpr double M = 16.0;  // Molar mass [g/mol]
};

struct Argon
{
   static const char* symbol() { return "Ar"; };
   static const unsigned   Z = 18;    // Atomic number, i.e. number of protons
   static const unsigned   A = 40;    // Total number of protons and neutrons
   static constexpr double M = 39.9;  // Molar mass [g/mol]
};

} // namespace PHY
