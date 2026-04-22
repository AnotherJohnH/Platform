#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(TRM INTERFACE)
   
target_include_directories(TRM INTERFACE ..)

if(BUILD_TESTING)
   add_subdirectory(test)
endif()
