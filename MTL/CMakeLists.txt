#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_subdirectory(${PDK_TARGET}/target)

target_sources(MTL PRIVATE
               MTL_init.cpp
               MTL_load.cpp
               MTL_main.cpp
               MTL_halt.cpp
               MTL_clock.cpp
               MTL_putch.cpp
               MTL_getch.cpp
               MTL_PS2KeyDecode.cpp
               PALVideo.cpp)

# This warning seems broken in gcc 10 and later
target_compile_options(MTL PUBLIC "$<$<COMPILE_LANGUAGE:C>:-Wno-stringop-overflow>")
target_compile_options(MTL PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:-Wno-stringop-overflow>")

target_compile_definitions(MTL PUBLIC MTL_TARGET=1)
target_compile_definitions(MTL PUBLIC MTL_${PDK_TARGET}=1)

target_include_directories(MTL PUBLIC ..)

target_link_libraries(MTL PUBLIC UCL)
