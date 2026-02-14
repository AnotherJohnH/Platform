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

class FirstOrder
{
public:
   FirstOrder(Type type_ = BYPASS)
   {
      setType(type_);
      zero();
   }

   void setType(Type type_)
   {
      type = type_;

      computeCoef();
   }

   void setFreq(float freq_)
   {
      float w0 = W0_1HZ * freq_;

      alpha = expf(-w0);

      computeCoef();
   }

   //! Reset filter state to zero
   void zero()
   {
      x[1] = y[1] = 0.f;
   }

   Signal operator()(Signal x_)
   {
      const Coef& c = coef[ping_pong];

      x[0] = x_;
      y[0] = c.b0 * x[0] + c.b1 * x[1] - c.a1 * y[1];

      x[1] = x[0];
      y[1] = y[0];

      return y[0];
   }

private:
   void computeCoef()
   {
      unsigned next = ping_pong ^ 1;
      Coef&    c    = coef[next];

      switch(type)
      {
      default:
      case OFF:
         c.a1 = 0.f;
         c.b0 = 0.f;
         c.b1 = 0.f;
         break;

      case BYPASS:
         c.a1 = 0.f;
         c.b0 = 1.f;
         c.b1 = 0.f;
         break;

      case LOPASS:
         c.a1 = -alpha;
         c.b0 = 1.f - alpha;
         c.b1 = 0.f;
         break;

      case HIPASS:
         c.a1 = -alpha;
         c.b0 = (1.f + alpha) / 2.f;
         c.b1 = -c.b0;
         break;
      }

      ping_pong = next;
   }

   struct Coef
   {
      Float a1{};
      Float b0{};
      Float b1{};
   };

   const float W0_1HZ{2.f * M_PI / SAMPLE_RATE};  //!< Angular frequency for 1Hz

   // Signal pipeline
   Signal x[2] = {}; //!< Input
   Signal y[2] = {}; //!< Output

   // Coefficients
   Coef             coef[2];
   volatile uint8_t ping_pong{0};

   // Configuration
   Type type{OFF};

   // Derived configuration
   float alpha{};
};

} // namespace Filter

} // namespace SIG
