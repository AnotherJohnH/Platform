# Hardware config picoX7

Hardware config to support a MIDI synthesizer with a 16-bit stereo DAC, 16x2 LCD and 2x7 segment LED. MIDI in can be both via a physical MIDI or over USB from a USB host.

The LCD, LED and physical MIDI-in are optional and will not block operation if not fitted.

![Prototype](docs/breadboard_v0.04.jpg)

[Schematic](docs/schematic_v0.05.pdf) for the above.

+ A PIO state machine is used to generate I2S (and another if required MCLK) for the I2S DACs
+ UART-0 used as a debug console (115200 baud 8-N-1)
+ UART-1 (RX) implements the optional physical MIDI-IN interface
+ The LCD and LED displays are optional and will not block operation if not fitted

## Config variations

| Config | Supported targets | Description |
|------------|--------|-------------|
| RPIPICO_I2S_DAC | rp2040, rp2350 | Generic I2S DAC |
| RPIPICO_WAVESHARE_REV2_1| rp2040, rp2350 | WAVESHARE Pico-Audio Rev2.1 I2S DAC (piggy back) |
| RPIPICO_PIMORONI_PICO_AUDIO | rp2040, rp2350 | Pimoroni Pico Audio I2S DAC (piggy back) |
| RPIPICO_PWM_DAC | rp2040, rp2350 | Simple Pulse Width Modulation DAC |
| PIMORONI_VGA_DEMO | rp2040 | Pimoroni VGA Demo Base |
| NATIVE | native | |

## Pin out

### I2S_DAC
To drive a generic I2S DACs from pins 31, 32 and 34.
```
                   +-----------+
(Debug) TX UART <- |  1     40 | <> vbus +5v
                   |  2     39 | <> vsys
            gnd ## |  3     38 | ## gnd
                   |  4     37 | <- 3v3-en
                   |  5     36 | -> 3v3
                   |  6     35 | <- adc-vref
                   |  7     34 | -> I2S LRCLK
            gnd ## |  8     33 | ## agnd
                   |  9     32 | -> I2S SCLK
                   | 10     31 | -> I2S SD
                   | 11     30 | <- run
                   | 12     29 |    
            gnd ## | 13     28 | ## gnd
                   | 14     27 | <- UART RX (MIDI in)
                   | 15     26 |    
                   | 16     25 | -> I2C SCL (16x2 LCD)
                   | 17     24 | -> I2C SDA (16x2 LCD)
            gnd ## | 18     23 | ## gnd
                   | 19     22 | -> 7-SEG LED clk
                   | 20     21 | -> 7-SEG LED data
                   +-----------+
```
### PWM_DAC
DAC implemented using the on-chip PWM with a few external resistors and capacitors on pin 31. Sound quality is poor!
```
                   +-----------+
(Debug) TX UART <- |  1     40 | <> vbus +5v
                   |  2     39 | <> vsys
            gnd ## |  3     38 | ## gnd
                   |  4     37 | <- 3v3-en
                   |  5     36 | -> 3v3
                   |  6     35 | <- adc-vref
                   |  7     34 |    
            gnd ## |  8     33 | ## agnd
                   |  9     32 |    
                   | 10     31 | -> PWM (audio)
                   | 11     30 | <- run
                   | 12     29 |    
            gnd ## | 13     28 | ## gnd
                   | 14     27 | <- UART RX (MIDI in)
                   | 15     26 |    
                   | 16     25 | -> I2C SCL (16x2 LCD)
                   | 17     24 | -> I2C SDA (16x2 LCD)
            gnd ## | 18     23 | ## gnd
                   | 19     22 | -> 7-SEG LED clk
                   | 20     21 | -> 7-SEG LED data
                   +-----------+
```
### WAVESHARE_REV2_1
Support for an I2S DAC based around the Cirrus Logic CS4344 on a WaveShare module. This DAC has the added requirement compare to other I2S DACs for an extra MCLK signal. Also note, Cirrus Logic have discontinued this device.

The pin allocations match the WaveShare module so that it can be used in a piggy back configuration.
```
                   +-----------+
(Debug) TX UART <- |  1     40 | <> vbus +5v
                   |  2     39 | <> vsys
            gnd ## |  3     38 | ## gnd
                   |  4     37 | <- 3v3-en
                   |  5     36 | -> 3v3
                   |  6     35 | <- adc-vref
                   |  7     34 | -> I2S SCLK
            gnd ## |  8     33 | ## agnd
                   |  9     32 | -> I2S LRCLK
                   | 10     31 | -> I2S MCLK
                   | 11     30 | <- run
                   | 12     29 | -> I2S SD
            gnd ## | 13     28 | ## gnd
                   | 14     27 | <- UART RX (MIDI in)
                   | 15     26 |    
                   | 16     25 | -> I2C SCL (16x2 LCD)
                   | 17     24 | -> I2C SDA (16x2 LCD)
            gnd ## | 18     23 | ## gnd
                   | 19     22 | -> 7-SEG LED clk
                   | 20     21 | -> 7-SEG LED data
                   +-----------+
```
### PIMORONI_PICO_AUDIO
The pin allocations match the Pimoroni module so that it can be used in a piggy back configuration.
```
                   +-----------+
(Debug) TX UART <- |  1     40 | <> vbus +5v
                   |  2     39 | <> vsys
            gnd ## |  3     38 | ## gnd
                   |  4     37 | <- 3v3-en
                   |  5     36 | -> 3v3
                   |  6     35 | <- adc-vref
                   |  7     34 |    
            gnd ## |  8     33 | ## agnd
                   |  9     32 |    
                   | 10     31 |    
                   | 11     30 | <- run
         I2S SD <- | 12     29 |    
            gnd ## | 13     28 | ## gnd
       I2S SCLK <- | 14     27 | <- UART RX (MIDI in)
      I2S LRCLK <- | 15     26 |    
                   | 16     25 | -> I2C SCL (16x2 LCD)
                   | 17     24 | -> I2C SDA (16x2 LCD)
            gnd ## | 18     23 | ## gnd
                   | 19     22 | -> 7-SEG LED clk
                   | 20     21 | -> 7-SEG LED data
                   +-----------+
```
### PIMORONI_VGA_DEMO
!!! Untested !!!
```
                   +-----------+
(Debug) TX UART <- |  1     40 |    
                   |  2     39 | <> vsys
            gnd ## |  3     38 | ## gnd
                   |  4     37 | <- 3v3-en
                   |  5     36 | -> 3v3
                   |  6     35 | <- adc-vref
                   |  7     34 | -> I2S LRCLK
            gnd ## |  8     33 | ## agnd
                   |  9     32 | -> I2S SCLK
                   | 10     31 | -> I2S SD
                   | 11     30 | <- run
                   | 12     29 |    
            gnd ## | 13     28 | ## gnd
                   | 14     27 |    
                   | 15     26 |    
                   | 16     25 |    
                   | 17     24 |    
            gnd ## | 18     23 | ## gnd
                   | 19     22 |    
                   | 20     21 |    
                   +-----------+
```

## Components

|Name|Description|Source|
|---|---|---|
|Raspberry Pi Pico|DIP module hosting an RP2040 MCU and 2 MiB Flash|[PiHut](https://thepihut.com/products/raspberry-pi-pico?variant=41925332566211)|
|Waveshare Pico Audio|I2S DAC module that directly mounts under the Pico|[PiHut](https://thepihut.com/products/pico-audio-audio-module-for-raspberry-pi-pico-inc-speakers)|
|MIDI controller|Music keyboard with actual MIDI out (MIDI over the USB connector also works)|Old Roland PC-200 works for me|
|MIDI-in adapter|An optically isolated 5-pin DIN to UART interface|3 resistors, a diode and an op-amp work well if the controller is battery powered ;-)|
|Reset button|Reset the Pico and allow Flashing|Why not included on the Pico module ?!@#|
|16x2 I2C LCD|Optional voice name display|[PiHut](https://thepihut.com/products/lcd1602-i2c-module)|
|Raspberry Pi Debug probe|Not essential but very helpful|[PiHut](https://thepihut.com/products/raspberry-pi-debug-probe)|

**NOTE** No particular affiliation with PiHut but they have a nice selection and excellent service
