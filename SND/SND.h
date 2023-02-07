//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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

#ifndef SND_H
#define SND_H

#include "Control.h"
#include "ControlOp.h"
#include "Signal.h"
#include "SignalOp.h"

#include "Component/AdditiveOsc.h"
#include "Component/Bias.h"
#include "Component/Clamp.h"
#include "Component/Delay.h"
#include "Component/Gain.h"
#include "Component/Gate.h"
#include "Component/NoiseOsc.h"
#include "Component/PulseOsc.h"
#include "Component/Switch.h"
#include "Component/WaveTableOsc.h"

#include "Filter/APFilter.h"
#include "Filter/FBCFilter.h"
#include "Filter/NPoleFilter.h"
#include "Filter/NZeroFilter.h"

#include "Interface/Microphone.h"
#include "Interface/Monitor.h"

#endif
