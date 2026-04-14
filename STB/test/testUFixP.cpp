//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "STB/UFixP.h"

#include "STB/Test.h"

TEST(STB_UFixP, iostream)
{
   STB::UFixP<16> value_16{1.99};
   std::cout << value_16 << std::endl;

   STB::UFixP<0> value_0{value_16};
   std::cout << value_0 << std::endl;

   STB::UFixP<1> value_1{value_16};
   std::cout << value_1 << std::endl;

   STB::UFixP<2> value_2{value_16};
   std::cout << value_2 << std::endl;

   STB::UFixP<3> value_3{value_16};
   std::cout << value_3 << std::endl;

   STB::UFixP<4> value_4{value_16};
   std::cout << value_4 << std::endl;

   STB::UFixP<5> value_5{value_16};
   std::cout << value_5 << std::endl;

   STB::UFixP<31> value_32{value_16};
   std::cout << value_32 << std::endl;
}

TEST(STB_UFixP, basic)
{
   STB::UFixP<16> fx_value{1.89};
   double fl_value = fx_value;
   EXPECT_NEAR(1.89, fl_value, 0.01);

   fx_value += 1.0;
   fl_value = fx_value;
   EXPECT_NEAR(2.89, fl_value, 0.01);

   fx_value *= 2.0;
   fl_value = fx_value;
   EXPECT_NEAR(5.78, fl_value, 0.01);

   fx_value /= 2.0;
   fl_value = fx_value;
   EXPECT_NEAR(2.89, fl_value, 0.01);

   fx_value -= 1.0;
   fl_value = fx_value;
   EXPECT_NEAR(1.89, fl_value, 0.01);
}
