# UCL - micro C library

Provides basic standard C library functionality that does not rely on hardware-specific support.
UCL builds as `libUCL.a` and links against **TGT**.
Also includes minimal C++ compatibility headers such as `cstdint`, `cstring`, `limits`, and `type_traits`.

|Header|Description|
|------|-----------|
|`ctype.h`|Character classification and conversion|
|`stdint.h`, `stddef.h`, `inttypes.h`, `limits.h`|Basic types and limits|
|`string.h`|String and memory functions|
|`UCL/stdio.h`|Minimal formatted output|
|`UCL/stdlib.h`|Conversions, `abs()` and pseudo-random numbers|
|`UCL/PrintF.h`|Internal formatted output helper|
