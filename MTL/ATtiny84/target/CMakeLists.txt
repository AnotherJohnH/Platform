#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            startup.s
            platform.cpp)

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=attiny84 -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

target_include_directories(MTL PUBLIC .)

target_link_libraries(MTL PUBLIC ${gcc_lib})
