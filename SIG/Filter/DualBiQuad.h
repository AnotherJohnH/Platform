//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cmath>

#include "SIG/Types.h"
#include "SIG/Filter/Type.h"

namespace SIG {

namespace Filter {

class DualBiQuad
{
public:
   DualBiQuad(Type type_ = BYPASS)
   {
      setType(type_);
   }

   void setType(Type type_)
   {
      type = type_;

      computeCoef();
   }

   void setFreq(Freq freq_)
   {
      Freq w0 = W0_1HZ * freq_;

      setSinCosW0(sinf(w0), cosf(w0));
   }

   void setSinCosW0(float sin_w0_, float cos_w0_)
   {
      sin_w0 = sin_w0_;
      cos_w0 = cos_w0_;
      alpha  = sin_w0 / (2 * q);

      computeCoef();
   }

   void setQ(float q_)
   {
      q     = q_;
      alpha = sin_w0 / (2 * q);

      computeCoef();
   }

   void setGain(float db_gain_)
   {
      db_gain = db_gain_;

      A          = powf(10, db_gain / 40);
      two_sqrt_A = 2 * sqrtf(A);

      computeCoef();
   }

   Signal operator()(Signal x_)
   {
      x[0] = x_;
      m[0] = b[0] * x[0] + b[1] * x[1] + b[2] * x[2]
                         - a[1] * m[1] - a[2] * m[2];

      x[2] = x[1];
      x[1] = x[0];

      y[0] = b[0] * m[0] + b[1] * m[1] + b[2] * m[2]
                         - a[1] * y[1] - a[2] * y[2];

      m[2] = m[1];
      m[1] = m[0];

      y[2] = y[1];
      y[1] = y[0];

      return y[0];
   }

private:
   void computeCoef()
   {
      float recip_a0;

      switch(type)
      {
      case OFF:
         a[1] = 0;
         a[2] = 0;
         b[0] = 0;
         b[1] = 0;
         b[2] = 0;
         break;

      case BYPASS:
         a[1] = 0;
         a[2] = 0;
         b[0] = 1;
         b[1] = 0;
         b[2] = 0;
         break;

      case LOPASS:
         recip_a0 = 1 / (1 + alpha);
         a[1] = (-2 * cos_w0) * recip_a0;
         a[2] = (1 - alpha) * recip_a0;
         b[0] = ((1 - cos_w0) / 2) * recip_a0;
         b[1] = (1 - cos_w0) * recip_a0;
         b[2] = b[0];
         break;

      case HIPASS:
         recip_a0 = 1 / (1 + alpha);
         a[1] = (-2 * cos_w0) * recip_a0;
         a[2] = (1 - alpha) * recip_a0;
         b[0] = ((1 + cos_w0) / 2) * recip_a0;
         b[1] = (-1 - cos_w0) * recip_a0;
         b[2] = b[0];
         break;

      case LOSHELF:
         recip_a0 = 1 / (A + 1 + (A - 1) * cos_w0 + two_sqrt_A * alpha);
         a[1] = (-2 * (A - 1 + (A + 1) * cos_w0)) * recip_a0;
         a[2] = ((A + 1) + (A - 1) * cos_w0 - two_sqrt_A * alpha) * recip_a0;
         b[0] = (A * (A + 1 - (A - 1) * cos_w0 + two_sqrt_A * alpha)) * recip_a0;
         b[1] = (2 * A * (A - 1 - (A + 1) * cos_w0)) * recip_a0;
         b[2] = (A * (A + 1 - (A - 1) * cos_w0 - two_sqrt_A * alpha)) * recip_a0;
         break;

      case HISHELF:
         recip_a0 = 1 / (A + 1 - (A - 1) * cos_w0 + two_sqrt_A * alpha);
         a[1] = (2 * (A - 1 - (A + 1) * cos_w0)) * recip_a0;
         a[2] = ((A + 1) - (A - 1) * cos_w0 - two_sqrt_A * alpha) * recip_a0;
         b[0] = (A * (A + 1 + (A - 1) * cos_w0 + two_sqrt_A * alpha)) * recip_a0;
         b[1] = (-2 * A * (A - 1 + (A + 1) * cos_w0)) * recip_a0;
         b[2] = (A * (A + 1 + (A - 1) * cos_w0 - two_sqrt_A * alpha)) * recip_a0;
         break;
      }
   }

   const Freq W0_1HZ{2 * M_PI / SAMPLE_RATE};  //!< Angular frequency for 1Hz

   // Configuration
   Type        type{OFF};
   float       q{1};
   float       db_gain{1};

   // Derived configuration
   float sin_w0{};
   float cos_w0{};
   float alpha{};
   float A{};
   float two_sqrt_A{};

   // Coefficients
   float a[3] = {};
   float b[3] = {};

   // Signal pipeline state
   Signal x[3] = {}; //!< Input
   Signal m[3] = {}; //!< Internal
   Signal y[3] = {}; //!< Output
};

} // namespace Filter

} // namespace SIG
