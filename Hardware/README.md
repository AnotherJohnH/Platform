# Hardware

## Configs

| Config | Docs   | Description |
|--------|--------|-------------|
| picoX7 | [README](blob/main/picoX7) | MIDI synthesizer with DAC, 16x2 LCD and 2x7 segment LED |
| picoX21H | [README](blob/main/picoX21H) | MIDI synthesizer with DAC, 16x2 LCD and an actual Yamaha YM2151 FM synthesizer chip. |
| picoSense | [README](blob/main/picoSense) | E-paper or LCD with I2C sensor. |
| picoGame | [README](blob/main/picoGame) | LCD with some buttons. |
| blinky | [README](blob/main/blinky) | Single LED. |

## Checkout

This repo uses git sub-modules, so checkout using --recurse-submodules to clone
all the dependent source...

    git clone --recurse-submodules https://github.com/AnotherJohnH/Hardware.git

or

    git clone --recurse-submodules ssh://git@github.com/AnotherJohnH/Hardware.git

## Software dependencies

+ https://github.com/AnotherJohnH/Platform
+ arm-none-eabi-gcc
+ cmake via UNIX make or auto detection of ninja if installed
+ Python3
+ SDL2 (only for native test target)

## Starting a new project using Hardware

```
mkdir MyProject
cd MyProject
git init .
git submodule add git@github.com:AnotherJohnH/Hardware.git
git submodule update --init --recursive
```

## Build

Being developed on MacOS but should build on Linux too.

Indirect build for supported targets, rp2040 and rp2350 with cmake and make (or ninja)...

    make

Build a single hardware target e.g. rp2350 using cmake...

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DPLT_TARGET=rp2350 -DCMAKE_TOOLCHAIN_FILE=Platform/MTL/rp2350/toolchain.cmake ..
    make

flashable test images will be found under the build sub-directory here...

    build/Source/picoX7Test_RPIPICO_I2S_DAC.uf2

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
