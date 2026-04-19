#-------------------------------------------------------------------------------
# Copyright (c) 2022 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

get_directory_property(has_parent PARENT_DIRECTORY)

if(has_parent)
   set(CMAKE_CXX_STANDARD          20   PARENT_SCOPE)
   set(CMAKE_CXX_STANDARD_REQUIRED True PARENT_SCOPE)
else()
   cmake_minimum_required(VERSION 3.19.1)

   project(PDK
           LANGUAGES CXX C ASM
           VERSION 0.5.0)
endif()

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

include(CTest)

#-------------------------------------------------------------------------------
# Introspect to determine PDK project variables...
#    PDK_TARGET
#    PDK_MACHINE
#    PDK_COMMIT

# Determine current git commit
execute_process(COMMAND git log --pretty=format:%H -n 1
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE PDK_COMMIT)

if(NOT PDK_TARGET)
   if(DEFINED ENV{PDK_TARGET})
      set(PDK_TARGET ${ENV{PLAT_TARGET})
   else()
      set(PDK_TARGET native)
   endif()
endif()

if(${PDK_TARGET} STREQUAL native)

   set(PDK_NATIVE  TRUE PARENT_SCOPE)
   set(PDK_NATIVE  TRUE)
   set(PDK_TARGET  ${CMAKE_HOST_SYSTEM_NAME})
   set(PDK_MACHINE ${CMAKE_HOST_SYSTEM_PROCESSOR})

   if(${PDK_TARGET} STREQUAL Darwin)
      set(PDK_TARGET macOS)
   endif()

else()
   set(PDK_NATIVE  FALSE PARENT_SCOPE)
   set(PDK_NATIVE  FALSE)
   set(CMAKE_ASM_FLAGS_DEBUG   "")
   set(CMAKE_ASM_FLAGS_RELEASE "")

   add_subdirectory(TNY)
   add_subdirectory(MTL)

endif()

#-------------------------------------------------------------------------------

function(arcxx name)
   add_custom_command(OUTPUT  arcxx_${name}.cpp
                      COMMAND ${CMAKE_SOURCE_DIR}/PDK/scripts/arcxx.py -o ${CMAKE_CURRENT_BINARY_DIR}/arcxx_${name}.cpp -r media/${name} ${CMAKE_SOURCE_DIR}/media/${name}
                      DEPENDS ${CMAKE_SOURCE_DIR}/media/${name})
endfunction()

#-------------------------------------------------------------------------------

function(autogen_py NAME)
   set(ARG "")
   if(ARGN)
      list(GET ARGN 0 ARG)
   endif()

   add_custom_command(
      COMMENT
         "Auto-generate ${NAME}"
      COMMAND
         ${CMAKE_COMMAND}
            -E env PYTHONPATH=${CMAKE_SOURCE_DIR}/PDK/python
            python3 ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.py ${ARG}
      DEPENDS
         ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.py
         ${INPUT_FILE}
      OUTPUT
         ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.h
         ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.cpp
      )

endfunction()

#-------------------------------------------------------------------------------

message("--------------------------------------------------------------------------------")
message("CMAKE_BUILD_TYPE   = ${CMAKE_BUILD_TYPE}")
message("PDK_COMMIT         = ${PDK_COMMIT}")
message("PDK_TARGET         = ${PDK_TARGET}")
message("PDK_MACHINE        = ${PDK_MACHINE}")
message("--------------------------------------------------------------------------------")

#-------------------------------------------------------------------------------

add_subdirectory(STB)
add_subdirectory(PLT)
add_subdirectory(USB)
add_subdirectory(SIG)
add_subdirectory(MIDI)
add_subdirectory(GUI)
add_subdirectory(TRM)
add_subdirectory(HWR)

