# HWR - Devices

## HWR::Led

Simple single LED interface that can be on or off.

```cpp
HWR::Led led;

// Turn LED on
led = true;

// Turn LED off
led = false;

// Implicit conversion to bool
bool is_on = led;

```
