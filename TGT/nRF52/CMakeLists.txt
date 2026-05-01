#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(TGT STATIC
            vector_table.s
            ../gcc_guard.cpp
            ../gcc_pure_virtual.cpp
            ../gcc_loader.cpp)

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=cortex-m4 -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

target_link_libraries(TGT PUBLIC MTL UCL ${gcc_lib})
