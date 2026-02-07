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
      const Coef& c = coef[ping_pong];

      x[0] = x_;
      m[0] = c.b0 * x[0] + c.b1 * x[1] + c.b2 * x[2]
                         - c.a1 * m[1] - c.a2 * m[2];

      x[2] = x[1];
      x[1] = x[0];

      y[0] = c.b0 * m[0] + c.b1 * m[1] + c.b2 * m[2]
                         - c.a1 * y[1] - c.a2 * y[2];

      m[2] = m[1];
      m[1] = m[0];

      y[2] = y[1];
      y[1] = y[0];

      return y[0];
   }

private:
   void computeCoef()
   {
      unsigned next = ping_pong ^ 1;
      Coef&    c    = coef[next];

      float recip_a0;

      switch(type)
      {
      case OFF:
         c.a1 = 0.0f;
         c.a2 = 0.0f;
         c.b0 = 0.0f;
         c.b1 = 0.0f;
         c.b2 = 0.0f;
         break;

      case BYPASS:
         c.a1 = 0.0f;
         c.a2 = 0.0f;
         c.b0 = 1.0f;
         c.b1 = 0.0f;
         c.b2 = 0.0f;
         break;

      case LOPASS:
         recip_a0 = 1 / (1 + alpha);
         c.a1 = (-2 * cos_w0) * recip_a0;
         c.a2 = (1 - alpha) * recip_a0;
         c.b0 = ((1 - cos_w0) / 2) * recip_a0;
         c.b1 = (1 - cos_w0) * recip_a0;
         c.b2 = c.b0;
         break;

      case HIPASS:
         recip_a0 = 1 / (1 + alpha);
         c.a1 = (-2 * cos_w0) * recip_a0;
         c.a2 = (1 - alpha) * recip_a0;
         c.b0 = ((1 + cos_w0) / 2) * recip_a0;
         c.b1 = (-1 - cos_w0) * recip_a0;
         c.b2 = c.b0;
         break;

      case LOSHELF:
         recip_a0 = 1 / (A + 1 + (A - 1) * cos_w0 + two_sqrt_A * alpha);
         c.a1 = (-2 * (A - 1 + (A + 1) * cos_w0)) * recip_a0;
         c.a2 = ((A + 1) + (A - 1) * cos_w0 - two_sqrt_A * alpha) * recip_a0;
         c.b0 = (A * (A + 1 - (A - 1) * cos_w0 + two_sqrt_A * alpha)) * recip_a0;
         c.b1 = (2 * A * (A - 1 - (A + 1) * cos_w0)) * recip_a0;
         c.b2 = (A * (A + 1 - (A - 1) * cos_w0 - two_sqrt_A * alpha)) * recip_a0;
         break;

      case HISHELF:
         recip_a0 = 1 / (A + 1 - (A - 1) * cos_w0 + two_sqrt_A * alpha);
         c.a1 = (2 * (A - 1 - (A + 1) * cos_w0)) * recip_a0;
         c.a2 = ((A + 1) - (A - 1) * cos_w0 - two_sqrt_A * alpha) * recip_a0;
         c.b0 = (A * (A + 1 + (A - 1) * cos_w0 + two_sqrt_A * alpha)) * recip_a0;
         c.b1 = (-2 * A * (A - 1 + (A + 1) * cos_w0)) * recip_a0;
         c.b2 = (A * (A + 1 + (A - 1) * cos_w0 - two_sqrt_A * alpha)) * recip_a0;
         break;
      }

      ping_pong = next;
   }

   struct Coef
   {
      Float a1{};
      Float a2{};
      Float b0{};
      Float b1{};
      Float b2{};
   };

   const Freq W0_1HZ{2 * M_PI / SAMPLE_RATE};  //!< Angular frequency for 1Hz

   // Signal pipeline
   Signal x[3] = {}; //!< Input
   Signal m[3] = {}; //!< Internal
   Signal y[3] = {}; //!< Output

   // Coefficients
   Coef             coef[2];
   volatile uint8_t ping_pong{0};

   // Configuration
   Type  type{OFF};
   Float q{1.0f};
   Float db_gain{1.0f};

   // Derived configuration
   Float sin_w0{};
   Float cos_w0{};
   Float alpha{};
   Float A{};
   Float two_sqrt_A{};
};

} // namespace Filter

} // namespace SIG
