//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C/C++ library implementation

#include <cstddef>

// XXX gcc is bringing in a dependency on the delete operator
//     when a virtual destructor is used. This is not expected
//     (by me) and have not dug into why it's there

// Nothing to do dynamic memory management is not expected

void operator delete(void*) {}
void operator delete(void*, size_t) {}

