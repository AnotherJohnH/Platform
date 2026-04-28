#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_subdirectory(${PDK_TARGET}/target)

target_sources(MTL PRIVATE
               MTL_init.cpp
               MTL_main.cpp
               MTL_halt.cpp
               MTL_clock.cpp
               MTL_putch.cpp
               MTL_getch.cpp
               MTL_PS2KeyDecode.cpp
               PALVideo.cpp)

target_compile_definitions(MTL PUBLIC MTL_TARGET=1)
target_compile_definitions(MTL PUBLIC MTL_${PDK_TARGET}=1)

target_include_directories(MTL PUBLIC ..)

target_link_libraries(MTL PUBLIC UCL)
