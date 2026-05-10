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
|`void MTL_halt(unsigned)`|Called when main() returns or the application explicitly requests to stop. Shuts down the system and wait for power off or reset.|
|`void MTL_fault(unsigned excep_num, unsigned long* stack_ptr)`|A fault or exception has occurred.|

## PDK macros

|Macro|Description|
|-----|-----------|
|PDK_PROJ_VERSION|Project version string, derived from **cmake** PROJECT_VERSION|
|PDK_PROJ_COMMIT|Head git commit hash at time of build|
|PDK_TARGET|PDK target name e.g. "rp2040"|
|PDK_MACHINE|Processor architecture e.g. "armv6m"|
|PDK_\<TARGET\>|PDK target name as a macro e.g. PDK_RP2040|

## Targets

|PDK_TARGET|PDK_MACHINE|Description|
|----------|------------|-----------|
|`native`|host|Host build using native compiler|
|`ATtiny84`|`avr24`|Atmel ATtiny84|
|`ATtiny85`|`avr25`|Atmel ATtiny85|
|`LPC810`|`armv6m`|NXP LPC810, Arm Cortex-M0|
|`LPC1114`|`armv6m`|NXP LPC1114, Arm Cortex-M0|
|`LPC11U24`|`armv6m`|NXP LPC11U24, Arm Cortex-M0|
|`LPC1768`|`armv7m`|NXP LPC1768, Arm Cortex-M3|
|`nRF51`|`armv6m`|Nordic nRF51 (micro:bit v1), Arm Cortex-M0|
|`nRF52`|`armv7m`|Nordic nRF52 (micro:bit v2), Arm Cortex-M4|
|`rp2040`|`armv6m`|Raspberry Pi RP2040 (Pico), dual Arm Cortex-M0+|
|`rp2350`|`armv8m`|Raspberry Pi RP2350 (Pico 2), dual Arm Cortex-M33|
