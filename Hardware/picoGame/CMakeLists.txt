#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_TARGET} STREQUAL rp2040)

   set(hw_picoGame_config_list
       TUFTY2040
       CACHE INTERNAL "picoGame config Variations")

elseif(${PLT_TARGET} STREQUAL native)

   set(hw_picoGame_config_list
       NATIVE
       CACHE INTERNAL "picoGame config Variations")

endif()

#-------------------------------------------------------------------------------
# Helper function for building binaries for each hardware config

function(add_picoGame_executable target_name)

    foreach(hw_config ${hw_picoGame_config_list})

        set(target ${target_name}_${hw_config})

        set(sources "")
        set(libs "")
        set(mode "source")

        foreach(arg IN LISTS ARGN)
           if (arg STREQUAL "LIBRARIES")
              set(mode "libs")
           elseif(mode STREQUAL "source")
               list(APPEND sources ${arg})
           else()
               list(APPEND libs ${arg})
           endif()
        endforeach()

        add_executable(${target} ${sources})
        target_include_directories(${target} PUBLIC  ${CMAKE_CURRENT_BINARY_DIR})
        target_compile_definitions(${target} PRIVATE HW_${hw_config})
        target_link_libraries(     ${target} PRIVATE ${libs} HW)

        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

    endforeach()

endfunction()

#-------------------------------------------------------------------------------
# Test

add_picoGame_executable(picoGameTest test.cpp)
add_picoGame_executable(picoGameEx   example.cpp)
