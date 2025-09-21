#-------------------------------------------------------------------------------
# Copyright (c) 2022 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# cmake configuration for Emscripten builds

add_library(plt_target_obj OBJECT 
            ../../SDL2/Audio.cpp
            ../../SDL2/Event.cpp
            ../../SDL2/Frame.cpp
            ../../SDL2/Image.cpp
            ../../SDL2/Bitmap.cpp
            ../../Stub/Midi.cpp
            ../../Stub/Sounder.cpp
            ../../Stub/Info.cpp
            ../../Stub/Yield.cpp
            ../../POSIX/Yield.cpp
            ../../POSIX/File.cpp)

target_compile_definitions(plt_target_obj
                           PUBLIC CO_OP_MULTI_TASK)

target_compile_options(plt_target_obj
                           PUBLIC -Wno-main -s USE_SDL=2)

target_include_directories(plt_target_obj
                           PUBLIC  ../native
                           PUBLIC  ../../../../include
                           PUBLIC  ../../../../include/MTL/board/stub
                           PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(plt_target_obj PUBLIC SDL2)

target_link_options(plt_target_obj PUBLIC -s USE_SDL=2)

