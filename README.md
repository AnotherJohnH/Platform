# Platform

A platform abstraction layer for building portable applications.

Provides APIs for...
* Low-level 2D rendering
* Simple GUIs
* Audio processing and synthesis
* MIDI
* Console applications
  * Command line processing
  * A light-weight curses API
* Some static data structures suitable for an embedded or limited memory enviornment

The intended deployment of Platform is as a submodule inside other projects.

## Starting a new project using Platform

```
mkdir MyProject
cd MyProject
git init .
git submodule add https://github.com/AnotherJohnH/Platform.git
cd Platform
./INSTALL_TEMPLATE

Project dir [/Users/fred/MyProjects/Foo] : <project root directory>
Copyright owner [John D. Haughton] : <copyright owner>
Copyright year [2017] : <copyright year>
User name [AnotherJohnH] : <GitHub user id>
Application [Foo] : <name of application>
Version [0.0.0] : <initial version>
Description [Empty application] : <application purpose>
Binary [foo] : <filename of application binary>

Install template/.clang-format to <project root directory>/.clang-format
Install template/.gitignore to <project root directory>/.gitignore
Install template/.travis.yml to <project root directory>/.travis.yml
Install template/Doxyfile to <project root directory>/Doxyfile
Install template/LICENSE to <project root directory>/LICENSE
Install template/README.md to <project root directory>/README.md
Install template/SConstruct to <project root directory>/SConstruct
Install template/Source/consoleApp.cpp to <project root directory>/Source/<filename of application binary>.cpp
```

```
cd ..
git add .
git commit -m Initial
scons
```

The following small projects are using Platform...
* [MineSweeper](https://github.com/AnotherJohnH/MineSweeper/)
* [Sternhalma](https://github.com/AnotherJohnH/Sternhalma/)
* [Zif](https://github.com/AnotherJohnH/Zif/)

## Status

[![Build Status](https://travis-ci.org/AnotherJohnH/Platform.svg?branch=master)](https://travis-ci.org/AnotherJohnH/Platform)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/13938/badge.svg)](https://scan.coverity.com/projects/anotherjohnh-platform)

### Linux and macOS

The primary target platforms at the moment.

Depend on SDL2, so a development install of SDL2 is required.

### Kindle3

Although this is also a Linux build, it does not depend on SDL2.

Requires gcc built for arm-linux-gnueabihf and a set of headers and static runtime libraries
that are compatible with the Linux installed on the Kindle3. The original ARMv6 Raspberry Pi
running a Debian based Linux has been found to be suitable platform to build the Kindle3 version.

### Browser (via Emscripten)

Uses the SDL2 library supplied with Emscripten.

### iOS

Requires access to a source installation of SDL2 which is built into a linSDL2.a as part of
the application build process.

Build of an actual app has not been achieved yet, and may not be easily achievable without
an Xcode project file.

### Android

Requires an NDK installation. Currently builds as a console only app as an integration with
an Android build of SDL2 has not been achieved yet.

## Coding style

The source is C++ but has the following non-typical for modern C++ features ...
* Memory is statically or stack allocated in preference to new/delete
* Use of C style stdio API
* In some places have re-invented the wheel avoiding functionality that is in standard libraries
* 3 space indent

