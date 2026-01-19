#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

set(SIG_SAMPLE_RATE "48000" CACHE STRING "SIG library sample rate (Hz)")

message("SIG_SAMPLE_RATE = ${SIG_SAMPLE_RATE} Hz")

autogen_py(Table_delta14_7 ${SIG_SAMPLE_RATE})
autogen_py(Table_atten15)

add_library(SIG STATIC
            Table_delta14_7.cpp
            Table_atten15.cpp)

target_include_directories(SIG PUBLIC .. ${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(SIG PUBLIC SIG_SAMPLE_RATE=${SIG_SAMPLE_RATE})

add_subdirectory(test)
