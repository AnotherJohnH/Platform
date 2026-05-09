# TGT - Target support

* Toolchain configuration
* Vector tables
* Loadable file preparation
* Some gcc support routines
* Definition of PDK macros for use in C/C++

## Toolchain configuration

Configures the C, C++, assembler and linker options, including invocation. Includes linker script for selected target and processing of linked image into a loadable file.

For example, to use the rp2350 toolchain configuration.
```
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=PDK/TGT/rp2350/toolchain.cmake
```

## Vector table objects

Note, the vector table objects depend on functions implemented in the target specific MTL library. To avoid circular dependencies those objects are not in **libTGT.a** but added to **libMTL.a**.

|Symbol|Description|
|------|-----------|
|`void MTL_init()`|Called from the reset vector after data and bss have been initialised, but before global constructors, should bring up and configure the microcontroller|
|`void MTL_halt(uint32_t)`|Called when main() returns or the application explicitly requests to stop. Shuts down the system and wait for power off or reset.|
|`void MTL_fault(unsigned excep_num, uint32_t* stack_ptr)`|A fault exception has occurred.|

## PDK macros

|Macro|Description|
|-----|-----------|
|PDK_PROJ_VERSION|Project version string, derived from **cmake** PROJECT_VERSION|
|PDK_PROJ_COMMIT|Head git commit hash at time of build|
|PDK_TARGET|PDK target name e.g. "rp2040"|
|PDK_MACHINE|Processor architecture e.g. "armv6m"|
|PDK_\<TARGET\>|PDK target name as a macro e.g. PDK_RP2040|

