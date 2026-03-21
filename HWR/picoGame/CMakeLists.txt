#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_TARGET} STREQUAL rp2040)

   set(hwr_picoGame_config_variants
       TUFTY2040
       CACHE INTERNAL "picoGame configVariants")

elseif(${PLT_NATIVE})

   set(hwr_picoGame_config_variants
       NATIVE
       CACHE INTERNAL "picoGame configVariants")

endif()

#-------------------------------------------------------------------------------
# Helper function for building binaries for each hardware config

function(add_picoGame_executable target_name)

    foreach(variant ${hwr_picoGame_config_variants})

        set(target ${target_name}_${variant})

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
        target_compile_definitions(${target} PRIVATE HW_${variant})
        target_link_libraries(     ${target} PRIVATE ${libs} GUI HWR)

        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

    endforeach()

endfunction()

#-------------------------------------------------------------------------------
# Test

add_picoGame_executable(picoGameTest test.cpp)
add_picoGame_executable(picoGameEx   example.cpp)
