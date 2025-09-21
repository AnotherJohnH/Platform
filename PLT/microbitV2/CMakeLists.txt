#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(PLT STATIC
            startup.s
            platform_clock.cpp
            platform_init.cpp
            platform_shutdown.cpp
            Frame.cpp
            Event.cpp
            Sounder.cpp
            ../Stub/Audio.cpp
            ../Stub/Image.cpp
            ../Stub/Bitmap.cpp
            ../Stub/Midi.cpp
            ../Stub/Yield.cpp
            ../Stub/Info.cpp
            ../Stub/File.cpp)

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=cortex-m3 -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

target_link_libraries(PLT PUBLIC ${gcc_lib} TNY)
