//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#pragma once

#define M_PI 3.141592653589793

#ifdef __cplusplus
extern "C" {
#endif

double fabs(double);
double sin(double);
double cos(double);
double pow(double, double);

#ifdef __cplusplus
}
#endif
