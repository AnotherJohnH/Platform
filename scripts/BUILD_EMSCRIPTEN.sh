#!/bin/echo "source this file then mkdir build; cd build; cmake ..; make"
#
# Setup environment for an Emscripten build

export PLT_TARGET=Emscripten

# XXX edit to the actual location that the EMSDK is installed
EMSDK_PATH=$HOME/OpenSource/emsdk_portable

echo
echo "EMSDK path    : ${EMSDK_PATH}"
echo
echo "NOTE: The EMSDK setup adds a special build of clang to PATH, which"
echo "will likely interfere with subsequent clang based builds for different"
echo "targets."
echo
echo "----------------------------------------------------------------------"
echo

source $EMSDK_PATH/emsdk_env.sh

