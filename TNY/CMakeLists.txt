#-------------------------------------------------------------------------------
# Copyright (c) 2022 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(TNY STATIC
            abort.cpp
            cos.c
            cosf.c
            clock.cpp
            delete.cpp
            exit.cpp
            expf.cpp
            fabsf.cpp
            fprintf.cpp
            feof.cpp
            fgetc.cpp
            fputc.cpp
            feof.cpp
            fstreams.cpp
            getchar.cpp
            global_obj.cpp
            logf.cpp
            malloc.cpp
            pow.c
            powf.cpp
            printf.cpp
            putchar.cpp
            puts.cpp
            sin.c
            sinf.c
            sqrt.c
            sqrtf.c
            scanf.cpp
            sleep.cpp
            system.cpp
            tanhf.c
            time.cpp
            usleep.cpp
            vfprintf.cpp)

target_include_directories(TNY PUBLIC include)

target_link_libraries(TNY PUBLIC MTL)
