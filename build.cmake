#-------------------------------------------------------------------------------
#  Copyright (c) 2019 John D. Haughton
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

# Construct a target platform specific build environment
# TODO this cmake support is under construction

project(${app})

execute_process(COMMAND git log --pretty=format:%H -n 1
                OUTPUT_VARIABLE commit)

add_compile_options(-DPROJ_COMMIT=\"${commit}\")
add_compile_options(-DPROJ_VERSION=\"${version}\")
add_compile_options(-DPROJ_MACHINE=\"?\")
add_compile_options(-Wall)
add_compile_options(-Werror)
add_compile_options(-std=c++11)

include_directories(Platform/include)

add_library(PLT
            Platform/Source/STB/Option.cpp)

add_executable(${binary} ${source})
target_link_libraries(${binary} PLT)
