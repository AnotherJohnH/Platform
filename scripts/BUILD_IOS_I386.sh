#!/bin/echo "source this file then type 'scons' to run SConstruct"
#
# Setup environment for iOS cross build for the iOS Simulator

export PROJ_TARGET=iOS

export IOS_PLATFORM=iPhoneSimulator
export IOS_ARCH=i386

# XXX edit to actual installed version
export IOS_VERSION=10.3

# XXX may need to edit to actual location
export IOS_XCODE_PATH=/Applications/Xcode.app/Contents/Developer

# XXX edit to actual installed source version and location
# Download from https://www.libsdl.org
export IOS_SDL2_PATH=$HOME/OpenSource/SDL2-2.0.5

export XCODE_DEVELOPER_USR_PATH=$IOS_XCODE_PATH/usr

PATH=$PATH:$IOS_XCODE_PATH/usr/bin

echo
echo "Target platform     : ${IOS_PLATFORM} v${IOS_VERSION}"
echo "Target architecture : ${IOS_ARCH}"
echo "XCode path          : ${IOS_XCODE_PATH}"
echo "SDL2 source path    : ${IOS_SDL2_PATH}"
echo
