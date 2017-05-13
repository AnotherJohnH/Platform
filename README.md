# Zif

A Z-code engine for playing interactive fiction games.
Perfectly good Z-code engines already exist.
This project is just for the fun of writing code and the learning that brings.

## Status

This application is currently alpha quality. It has been built and seen to run on various platforms. But significant issues need to be addressed before it can be used for it's intended purpose.

## How to build

The application is built using the SConstruct file in the top level directory. Before building the SConstruct file should be edited to uncomment the intended target platform.

### Linux and macOS

Depends on SDL2. An installation of SDL2 header and library files is assumed to be in /use/local/include and /usr/local/lib respectively.

### Kindle3

Depends on gcc built for arm-linux-gnueabihf and a set of headers and static runtime libraries that are compatible with the Linux installed on the Kindle3. The original ARMv6 Raspberry Pi has been found to be suitable.

### Windows

Not supported. (and not interested)

## How to run

## Coding style

The source is C++ but has the following non-typical for C++ features ...
* All memory is statically or stack allocated i.e. no new/delete, direct or hidden
* Use of C style stdio API - just prefer it
