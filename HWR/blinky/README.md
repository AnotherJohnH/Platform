# HWR config blinky

Hardware config for projects based around a single LED.

Useful as a "Hello, world!" test for new targets and variants.

## blinky config variations

| PLT_TARGET | Variant    | Description |
|------------|------------|-------------|
| rp2040     | RPIPICO    | Raspberry-Pi pico module |
| rp2040     | BADGER2040 | Pimoroni badger2040 |
| rp2040     | TUFTY2040  | Pimoroni tufty2040 |
| rp2350     | RPIPICO    | Raspberry-Pi pico2 module |
| LPC11U24   | MBED       | mbed module |
| LPC1768    | MBED       | mbed module |
| nrF51      | MICROBIT   | microbit board |
| nrF52      | MICROBIT   | microbit board |
| macOS      | NATIVE     | Host OS |
| Linux      | NATIVE     | Host OS |

## Example

```cpp
// Flash LED at ~1 Hz

#include <unistd.h>

#include "HWR/blinky/Config.h"

static HWR::Led led;

int main()
{
   led = false;

   while(true)
   {
      led = not led;

      usleep(500000);
   }

   return 0;
}
```
