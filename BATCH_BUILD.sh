#!/bin/sh

# macOS batch build for multiple targets
#
# Requires...
#    SDL2 development install  (native build)
#    SDL2 source               (iOS builds)
#    NDK                       (Android builds)
#    EMSDK                     (Emscripten builds)

for build in NATIVE \
             IOS_ARMv7 IOS_ARM64 IOS_I386 IOS_X86_64 \
             ANDROID_ARM ANDROID_ARM64 \
             EMSCRIPTEN
do
   source BUILD_${build}.sh
   scons $1
done

