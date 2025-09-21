#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_executable(testSND_Expr testSND_Expr.cpp)
target_link_libraries(testSND_Expr PLT)

add_executable(testSND_Sine testSND_Sine.cpp)
target_link_libraries(testSND_Sine PLT)
