#-------------------------------------------------------------------------------
#  Copyright (c) 2022 John D. Haughton
# 
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
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

