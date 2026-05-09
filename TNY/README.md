# TNY - Tiny C library

Provides further standard C library functionality over `UCL` that relies on hardware-specific
support from `MTL`.
TNY builds as `libTNY.a` and links against `MTL`.
Also includes minimal C++ compatibility headers such as `algorithm`, `array`, `cmath`, `cstdio`,
`cstdlib`, `ctime`, `mutex`, and `string`.

|Header|Description|
|------|-----------|
|`assert.h`|Runtime assertions|
|`math.h`|Small math function subset|
|`signal.h`|Signal registration and raising|
|`stdio.h`|Console and file-style I/O|
|`stdlib.h`|Allocation, process termination, and system hooks|
|`termios.h`|Terminal attribute access|
|`time.h`|Clock and time functions|
|`unistd.h`|Sleep functions|

## Status

This is a work in progress and some functionality is incomplete.
