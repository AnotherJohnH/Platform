#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_executable(testUCL
               test_main.cpp
               test_ctype.cpp)

target_link_libraries(testUCL PRIVATE UCL)

add_test(NAME testUCL COMMAND testUCL)
