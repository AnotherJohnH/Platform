# Zif

A Z-code engine for playing interactive fiction games.

Portability of the application between different platforms is a particular goal, and specifically
portability to the third generation Kindle (aka Kindle Keyboard) which appears as an ideal platform
for playing interactive fiction.

The application includes a terminal emulator and a basic curses style front-end menu, so when running
on the Kindle a third-party terminal emulator and launcher is not necessary.

Perfectly good Z-code engines already exist and some have already been ported to the Kindle.
This project is just for the fun of writing code and the learning that brings.

## Status

The application is currently alpha quality. It has been built and seen to run on various platforms.
Many popular Z-code games downloaded from the interactive fiction archive start to run as expected
although further testing is necessary. But a significant number of issues need to be addressed before
zif is ready for it's intended purpose.

## How to build

The application is built using the SConstruct file in the top level directory.
Before building the SConstruct file should be edited to uncomment the intended target platform.

### Linux and macOS

Depends on SDL2. An installation of SDL2 header and library files is assumed to be in /use/local/include
and /usr/local/lib respectively.

### Kindle3

Depends on gcc built for arm-linux-gnueabihf and a set of headers and static runtime libraries that are
compatible with the Linux installed on the Kindle3. The original ARMv6 Raspberry Pi has been found to be
suitable platform to build the Kindle3 version.

### Windows

Not supported. (and no desire to support)

## How to run

From the command line the -h or --help option provides basic help. Supplying a .zN game file as a
command line argument should load and run the game file.

Alternatively the zif.cfg file is used to provide a menu front end, where z-code games available in
the local file system can be navigated to and certain aspects of the terminal emulation configured.

## Coding style

The source is C++ but has the following non-typical for modern C++ features ...
* All memory is statically or stack allocated i.e. no new/delete, direct or hidden
* Use of C style stdio API - just prefer it
