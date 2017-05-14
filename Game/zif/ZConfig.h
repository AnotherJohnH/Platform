//------------------------------------------------------------------------------
// Copyright (c) 2016-2017 John D. Haughton
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

#ifndef ZCONFIG_H
#define ZCONFIG_H

//! Configuration/features of the interpreter
struct ZConfig
{
   unsigned  interp_major_version{0};
   unsigned  interp_minor_version{0};
   bool      status_line{false};          //!< Status line available
   bool      screen_splitting{false};     //!< Screen splitting available
   bool      var_pitch_font{false};       //!< variable pitch font the default
   bool      pictures{false};             //!< picture display available
   bool      sounds{false};               //!< sounds available
   bool      timed_keyboard{false};       //!< timed keyboard input available
   bool      transcripting{false};        //!< transcripting starts on
};

#endif

