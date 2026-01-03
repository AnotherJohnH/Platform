#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_TARGET} STREQUAL rp2040)

   set(hw_picoX21H_config_list
       RPIPICO_I2S_DAC
       RPIPICO_PWM_DAC
       RPIPICO_WAVESHARE_REV2_1
       CACHE INTERNAL "picoX21 config variations")

elseif(${PLT_TARGET} STREQUAL rp2350)

   set(hw_picoX21H_config_list
       RPIPICO_I2S_DAC
       RPIPICO_PWM_DAC
       RPIPICO_WAVESHARE_REV2_1
       CACHE INTERNAL "picoX21H config variations")

elseif(${PLT_TARGET} STREQUAL native)

   set(hw_picoX21H_config_list
       NATIVE
       CACHE INTERNAL "picoX21H config Variations")

endif()

#-------------------------------------------------------------------------------
# Helper function for building binaries for each hardware config

function(add_picoX21H_executable target_name)

    foreach(hw_config ${hw_picoX21H_config_list})

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

autogen_py(Table_sine)

add_picoX21H_executable(picoX21HTest test.cpp Table_sine.cpp)
