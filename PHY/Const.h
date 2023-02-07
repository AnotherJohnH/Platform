//------------------------------------------------------------------------------
// Copyright (c) 2013-2019 John D. Haughton
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

#ifndef PHY_CONST_H
#define PHY_CONST_H

namespace PHY {

namespace Const {

static const double c  = 299792458;        //!< Speed of light in a vacuum [m/s]
static const double L  = 6.022045e23;      //!< Avogadro constant          [/mol]
static const double e  = 1.6021892e-19;    //!< Elementary charge          [C]
static const double kb = 1.3806485279e-23; //!< Boltzmann constant         [J/K]
static const double g  = 9.80665;          //!< standard gravity           [m/s^2]
static const double G  = 6.6720e-11;       //!< Gravitational constant     [N.m^2/kg^2]
static const double h  = 6.626176e-34;     //!< Planck constant            [J/Hz]
static const double mp = 1.6726485e-27;    //!< Mass of proton             [kg]
static const double mn = 1.6749543e-27;    //!< Mass of neutron            [kg]
static const double me = 9.109534e-31;     //!< Mass of electron           [kg]

} // Const

} // PHY

#endif
