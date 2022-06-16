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

include_guard(GLOBAL)

#-------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD          14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#-------------------------------------------------------------------------------
# Introspect to determine Platform project variables...
#    PLT_TARGET
#    PLT_MACHINE
#    PLT_COMMIT

# Determine the target platform type
if(NOT PLT_TARGET)

   if(DEFINED ENV{PLT_TARGET})
      set(plt_target $ENV{PLT_TARGET})
   endif()

   if("${plt_target}" STREQUAL "" OR "${plt_target}" STREQUAL native)

      set(plt_target ${CMAKE_HOST_SYSTEM_NAME})

      if(${plt_target} STREQUAL Darwin)
          set(plt_target macOS)
      endif()

   else()

      set(CMAKE_CROSSCOMPILING TRUE)

      include(Platform/Source/PLT/target/${plt_target}/config.cmake)

   endif()

   set(PLT_TARGET ${plt_target} CACHE STRING "Choose the target type")

endif()

# Determine current git commit
execute_process(COMMAND git log --pretty=format:%H -n 1
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE PLT_COMMIT)

message("--------------------------------------------------------------------------------")
message("PLT_TARGET  = ${PLT_TARGET}")
message("PLT_MACHINE = ${CMAKE_SYSTEM_PROCESSOR}")
message("PLT_COMMIT  = ${PLT_COMMIT}")
message("--------------------------------------------------------------------------------")

#-------------------------------------------------------------------------------
# Toolchain setup

set(PLT_C_FLAGS)
list(APPEND PLT_C_FLAGS "-DPLT_TARGET_${PLT_TARGET}")
list(APPEND PLT_C_FLAGS "-DPLT_MACHINE=\"${CMAKE_SYSTEM_PROCESSOR}\"")
list(APPEND PLT_C_FLAGS "-DPLT_PROJ_COMMIT=\"${PLT_COMMIT}\"")
list(APPEND PLT_C_FLAGS "-DPLT_PROJ_VERSION=\"${PROJECT_VERSION}\"")
list(APPEND PLT_C_FLAGS "-Wall")
list(APPEND PLT_C_FLAGS "-Werror")

# TODO for bare metal targets
#set(CMAKE_ASM_FLAGS_RELEASE  ${PLT_asm_flags})
#set(CMAKE_C_FLAGS_RELEASE    "-DNDEBUG -O3 ${PLT_c_flags}")
#set(CMAKE_CXX_FLAGS_RELEASE  "-DNDEBUG -O3 ${PLT_cxx_flags} ${PLT_c_flags}")

#set(CMAKE_ASM_FLAGS_DEBUG    ${PLT_asm_flags})
#set(CMAKE_C_FLAGS_DEBUG      "-g -O0 ${PLT_c_flags}")
#set(CMAKE_CXX_FLAGS_DEBUG    "-g -O0 ${PLT_cxx_flags} ${PLT_c_flags}")

#-------------------------------------------------------------------------------

if(NOT ${PROJECT_NAME} STREQUAL PLT)
   add_subdirectory(Platform)
endif()
