#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(STB STATIC
            $<$<BOOL:${PDK_NATIVE}>:Deflate.cpp>
            $<$<BOOL:${PDK_NATIVE}>:Oil.cpp>
            Option.cpp
            Zlib.cpp)

target_include_directories(STB PUBLIC ..)

if(NOT PDK_NATIVE)
   target_link_libraries(STB PUBLIC TNY)
endif()

if(BUILD_TESTING)
   add_subdirectory(test)
endif()
