# Hardware config picoSense

Hardware config to support E-paper or LCD with an I2C sensor.

![Prototype](docs/badger_2040.jpg)

![Prototype](docs/breadboard_v0.01.jpg)

[Schematic](docs/schematic_v0.01.pdf) for the above.

## Config variations

| Config | Supported targets | Description |
|------------|--------|-------------|
| BADGER2040 | rp2040 | Pimoroni badger2040 |
| TUFTY2040 | rp2040 | Pimoroni tufty2040 |
| RPIPICO_WAVESHARE_EPAPER | rp2040 | WAVESHARE I2C EPAPER |
| NATIVE | native | |

## Pin out

### BADGER2040
```
                          +-----------+
       (Debug) TX UART <- |  1     40 | <> vbus +5v
                          |  2     39 | <> vsys
                   gnd ## |  3     38 | ## gnd
                          |  4     37 | <- 3v3-en
                          |  5     36 | -> 3v3
(temp) I2C SDA MCP9808 <> |  6     35 | <- adc-vref
(temp) I2C SCL MCP9808 <- |  7     34 |    
                   gnd ## |  8     33 | ## agnd
                          |  9     32 |    
                          | 10     31 | <- BSY (e-paper)
                          | 11     30 | <- run
                          | 12     29 |    
                   gnd ## | 13     28 | ## gnd
                          | 14     27 | -> RST (e-paper)
                          | 15     26 | -> DC  (e-paper)
                          | 16     25 | -> SPI MOSI (e-paper)
                          | 17     24 | -> SPI CLK  (e-paper)
                   gnd ## | 18     23 | ## gnd
                          | 19     22 | -> SPI CS   (e-paper)
                          | 20     21 | <- SPI MISO (e-paper)
                          +-----------+
```
### TUFTY2040
```
                          +-----------+
       (Debug) TX UART <- |  1     40 | <> vbus +5v
                          |  2     39 | <> vsys
                   gnd ## |  3     38 | ## gnd
                          |  4     37 | <- 3v3-en
                          |  5     36 | -> 3v3
(temp) I2C SDA MCP9808 <> |  6     35 | <- adc-vref
(temp) I2C SCL MCP9808 <- |  7     34 |    
                   gnd ## |  8     33 | ## agnd
             BTN5 (DN) <> |  9     32 |    
              BTN1 (A) <> | 10     31 |    
              BTN2 (B) <> | 11     30 | <- run
              BTN3 (C) <> | 12     29 | <>  BTN4 (UP)
                   gnd ## | 13     28 | ## gnd
              CS (LCD) <- | 14     27 | <> DB7 (LCD)
              RS (LCD) <- | 15     26 | <> DB6 (LCD)
              WR (LCD) <- | 16     25 | <> DB5 (LCD)
              RD (LCD) <- | 17     24 | <> DB4 (LCD)
                   gnd ## | 18     23 | ## gnd
             DB0 (LCD) <> | 19     22 | <> DB3 (LCD)
             DB1 (LCD) <> | 20     21 | <> DB2 (LCD)
                          +-----------+
```
### WAVESHARE_EPAPER
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
(e-paper) SPI MISO -> | 11     30 | <- run
(e-paper) SPI   CS <- | 12     29 |    
               gnd ## | 13     28 | ## gnd
(e-paper) SPI  CLK <- | 14     27 |    
(e-paper) SPI MOSI <- | 15     26 |    
     (e-paper) RST <- | 16     25 |    
     (e-paper) BSY -> | 17     24 |    
               gnd ## | 18     23 | ## gnd
                      | 19     22 | -> I2C SCL MCP9808 (temp sense)
                      | 20     21 | <> I2C SDA MCP9808 (temp sense)
                      +-----------+
```
