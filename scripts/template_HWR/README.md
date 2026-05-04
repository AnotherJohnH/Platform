`#' APPLICATION

DESCRIPTION.

## Status

[![Build Status](https://github.com/USERNAME/APPLICATION/actions/workflows/build_on_push.yml/badge.svg)](https://github.com/USERNAME/APPLICATION/actions/workflows/build_on_push.yml)

## How to build

Indirect build for all targets using the make helper...

```bash
make
```

Indirect build for a named target using the make helper...

```bash
make rp2040
```

Direct build for a single target using `CMake`...

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=PDK/TGT/rp2040/toolchain.cmak
make
```

Flashable images for modules supported for the selected target will be found under the build directory
