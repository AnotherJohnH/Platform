//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#pragma once

#define M_PI   3.14159265358979323846
#define M_PI_2 (M_PI / 2)
#define M_1_PI (1 / M_PI)

#define M_PI_F   3.14159265358979323846f
#define M_PI_2_F (M_PI_F / 2)
#define M_1_PI_F (1 / M_PI_F)

#ifdef __cplusplus
extern "C" {
#endif

double fabs(double);
double sin(double);
double cos(double);
double pow(double, double);
double sqrt(double);

float sinf(float);
float cosf(float);
float powf(float, float);
float sqrtf(float);

#ifdef __cplusplus
}
#endif
