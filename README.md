# Raspberry Pi Pico 2 NVIC example

This example demonstrates how to use the NVIC (nested vectored interrupt controller) in the ARM Cortex-M microcontroller processors, such as the Cortex-M33 found in the dual-core RP2350 SoC used by the Raspberry Pi Pico 2. It essentially performs hardware-accelerated interrupt servicing by fetching the ISR address directly from a hardware vector table. This differs from the GIC (generic interrupt controller) found in Cortex-A microprocessors, where a single generic interrupt vector receives all interrupts, and software is expected to read GIC registers to find the specific interrupt ID and then the ISR. The GIC is better suited for multi-core environments.

Instructions:

- Run `git submodule update --init --recursive`
- Run `sed -i "s/-DPICOTOOL_NO_LIBUSB=1/-DPICOTOOL_NO_LIBUSB=0/g" pico-sdk/tools/Findpicotool.cmake`
- Run `sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib`

To generate Makefiles:

```
PICO_SDK_PATH=$(git rev-parse --show-toplevel)/pico-sdk \
cmake -DPICO_BOARD=pico2 -DPICOTOOL_FORCE_FETCH_FROM_GIT=ON -S . -B build
```

To build application and run on board: 

- Unplug USB cable from board
- Hold down BOOTSEL button while plugging in USB cable
- Run `cmake --build build --target run`
