#!/bin/echo "source this file then type 'scons' to run SConstruct"
#
# Setup environment for microbit build
#

export PROJ_TARGET=microbit

# NOTE: some microbits use an nRF51 part with 32k RAM not 16K RAM
#export MICROBIT_RAM=32k
export MICROBIT_RAM=16k

