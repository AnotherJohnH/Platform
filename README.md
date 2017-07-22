# Platform

A platform abstraction layer for building portable applications.

## Starting a new project

```
mkdir MyProject
cd MyProject
git init .
git submodule add https://github.com/AnotherJohnH/Platform.git
cp Platform/template/* .
cp -r Platform/template/* .
```

Edit the copies of README.md and LICENSE as appropriate.

```
git add * .gitignore .travis.yml
git commit -m "Initial commit"
scons
```

## Status

[![Build Status](https://travis-ci.org/AnotherJohnH/Platform.svg?branch=master)](https://travis-ci.org/AnotherJohnH/Platform)

### Linux and macOS

Depend on SDL2, so a development install of SDL2 is required.

### Kindle3

Although this is also a Linux build, it does not depend on SDL2.

Requires gcc built for arm-linux-gnueabihf and a set of headers and static runtime libraries
that are compatible with the Linux installed on the Kindle3. The original ARMv6 Raspberry Pi
running a Debian based Linux has been found to be suitable platform to build the Kindle3 version.

### Browser (via Emscripten)

Uses the SDL2 library supplied with Emscripten. Some re-work/conditonal compilation will
be required to make it work.

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
* Memory is statically or stack allocated i.e. (almost) no new/delete
* Use of C style stdio API
* In some places have re-invented the wheel avoiding functionality that is in standard libraries
* 3 space indent
