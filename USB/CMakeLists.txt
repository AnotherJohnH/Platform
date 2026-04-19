#-------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(USB INTERFACE)

target_include_directories(USB INTERFACE ..)

add_subdirectory(test)
