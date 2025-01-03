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

cmake_minimum_required(VERSION 3.19.1)

project(Platform
        LANGUAGES CXX C ASM
        VERSION 0.5.0)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT PROJECT_IS_TOP_LEVEL)
   set(CMAKE_CXX_STANDARD          17   PARENT_SCOPE)
   set(CMAKE_CXX_STANDARD_REQUIRED True PARENT_SCOPE)
endif()

include(CTest)

#-------------------------------------------------------------------------------
# Introspect to determine Platform project variables...
#    PLT_TARGET
#    PLT_MACHINE
#    PLT_COMMIT

# Determine current git commit
execute_process(COMMAND git log --pretty=format:%H -n 1
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE PLT_COMMIT)

if(NOT PLT_TARGET)
   if(DEFINED ENV{PLT_TARGET})
      set(PLT_TARGET ${ENV{PLAT_TARGET})
   else()
      set(PLT_TARGET native)
   endif()
endif()

if(${PLT_TARGET} STREQUAL native)

   set(PLT_NATIVE  TRUE)
   set(PLT_TARGET  ${CMAKE_HOST_SYSTEM_NAME})
   set(PLT_MACHINE ${CMAKE_HOST_SYSTEM_PROCESSOR})

   if(${PLT_TARGET} STREQUAL Darwin)
      set(PLT_TARGET macOS)
   endif()

else()
   set(PLT_NATIVE  FALSE)
   set(CMAKE_ASM_FLAGS_DEBUG   "")
   set(CMAKE_ASM_FLAGS_RELEASE "")

   add_subdirectory(TNY)
   add_subdirectory(MTL)

endif()

message("--------------------------------------------------------------------------------")
message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message("PLT_COMMIT       = ${PLT_COMMIT}")
message("PLT_TARGET       = ${PLT_TARGET}")
message("PLT_MACHINE      = ${PLT_MACHINE}")
message("--------------------------------------------------------------------------------")

#-------------------------------------------------------------------------------

add_subdirectory(GUI)
add_subdirectory(STB)
add_subdirectory(PLT)

#-------------------------------------------------------------------------------

function(arcxx name)
   add_custom_command(OUTPUT  arcxx_${name}.cpp
                      COMMAND ${CMAKE_SOURCE_DIR}/Platform/scripts/arcxx.py -o ${CMAKE_CURRENT_BINARY_DIR}/arcxx_${name}.cpp -r media/${name} ${CMAKE_SOURCE_DIR}/media/${name}
                      DEPENDS ${CMAKE_SOURCE_DIR}/media/${name})
endfunction()
