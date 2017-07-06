#!/bin/echo "source this file then type 'scons' to run SConstruct"
#
# Setup environment for Android ARM A64 cross build

export PROJ_TARGET=Android

# XXX edit to actual installed version and location
export NDK_HOME=$HOME/OpenSource/android-ndk-r14b
export NDK_GCC_VER=4.9

# AArch64 builds
export NDK_API=21
export NDK_ARCH=arm64

# XXX edit for actual host
HOST_ARCH=darwin-x86_64

PATH=$PATH:$NDK_HOME/toolchains/aarch64-linux-android-${NDK_GCC_VER}/prebuilt/${HOST_ARCH}/bin

echo
echo "NDK path            : ${NDK_HOME}"
echo "NDK GCC version     : ${NDK_GCC_VER}"
echo "NDK API             : ${NDK_API}"
echo
echo "Host architecture   : ${HOST_ARCH}"
echo "Target architecture : ${NDK_ARCH}"
echo
