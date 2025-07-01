//------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

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
