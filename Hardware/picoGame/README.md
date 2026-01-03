# Hardware config picoGame

Hardware config to support LCD with some buttons.

## Config variations

| Config | Supported targets | Description |
|------------|--------|-------------|
| TUFTY2040 | rp2040 | Pimoroni tufty2040 |
| NATIVE | native | |

## Pin out

### TUFTY2040
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
