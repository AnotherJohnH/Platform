# HWR - Hardware library

The HWR library enables projects with similar hardware requirements to share common C++ APIs, allowing rapid development of new projects and portability of a project codebase between different hardware variants.

The library also supports native variants for macOS and Linux targets, where physical hardware I/O is simulated for debugging and quick evaluation.

## Configs

| HWR_CONFIG             | Platform I/O |
|------------------------|--------------|
| [blinky](blinky)       | Single LED   |
| [picoX7](picoX7)       | MIDI synthesizer with DAC, 16x2 LCD and 2x7 segment LED |
| [picoX21H](picoX21H)   | MIDI synthesizer with DAC, 16x2 LCD and an actual Yamaha YM2151 FM synthesizer chip. |
| [picoSense](picoSense) | E-paper or LCD with I2C sensor. |
| [picoGame](picoGame)   | LCD with some buttons. |

## Usage

The hardware config for a project is selected in the top level `CMakeLists.txt` before the `PDK` sub-directory is added.

```cmake
set(HWR_CONFIG picoX7)

add_subdirectory(PDK)
```

the C++ APIs are then accessible in the project source with:

```cpp
#include "HWR/picoX7/Config.h"
```

The cmake build targets for the config are then defined using the CMake helper for the selected config:

```cmake
add_picoX7_executable(picoSynth
                         picoSynth.cpp
                      LIBRARIES
                         Acorn
                         Casio
                         Roland
                         Experiment)
```

this will build all available variants for the current target including test images.
