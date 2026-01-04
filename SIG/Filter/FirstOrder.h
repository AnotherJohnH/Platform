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

   Signal operator()(Signal x_)
   {
      x[0] = x_;
      y[0] = b[0] * x[0] + b[1] * x[1] + a[1] * y[1];

      x[1] = x[0];
      y[1] = y[0];

      return y[0];
   }

private:
   void computeCoef()
   {
      switch(type)
      {
      default:
      case OFF:
         a[1] = 0;
         b[0] = 0;
         b[1] = 0;
         break;

      case BYPASS:
         a[1] = 0;
         b[0] = 1;
         b[1] = 0;
         break;

      case LOPASS:
         a[1] = alpha;
         b[0] = 1.0 - alpha;
         b[1] = 0.0;
         break;

      case HIPASS:
         a[1] = -alpha;
         b[0] = alpha;
         b[1] = -alpha;
         break;
      }
   }

   const float W0_1HZ{2 * M_PI / SAMPLE_RATE};  //!< Angular frequency for 1Hz

   // Configuration
   Type type{OFF};

   // Derived configuration
   float alpha{};

   // Coefficients
   float a[2] = {};
   float b[2] = {};

   // Signal pipeline
   Signal x[2] = {}; //!< Input
   Signal y[2] = {}; //!< Output
};

} // namespace Filter

} // namespace SIG
