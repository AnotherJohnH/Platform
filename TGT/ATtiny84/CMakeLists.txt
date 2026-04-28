#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(TGT STATIC
            vector_table.s
            ../TGT_load.cpp)

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=attiny84 -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

target_link_libraries(TGT PUBLIC MTL UCL ${gcc_lib})
