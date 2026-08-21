# Raspberry Pi Pico 2 NVIC example

This example demonstrates how to use the NVIC (nested vectored interrupt controller) in the ARM Cortex-M microcontroller processors, such as the Cortex-M33 found in the dual-core RP2350 SoC used by the Raspberry Pi Pico 2. It essentially performs hardware-accelerated interrupt servicing by fetching the ISR address directly from a hardware vector table. This differs from the GIC (generic interrupt controller) found in Cortex-A microprocessors, where a single generic interrupt vector receives all interrupts, and software is expected to read GIC registers to find the specific interrupt ID and then the ISR. The GIC is better suited for multi-core environments.

Instructions:

- Run `git submodule update --init --recursive`
- Run `sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib`

To generate Makefiles:

```
PICO_SDK_PATH=$(git rev-parse --show-toplevel)/pico-sdk \
cmake -DPICO_BOARD=pico2 \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/arm-none-eabi-gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/arm-none-eabi-g++ \
    -DPICOTOOL_FORCE_FETCH_FROM_GIT=ON \
    -S . -B build
```

To build application: `cmake --build build`

To run on board: 

- Unplug USB cable from board
- Hold down BOOTSEL button while plugging in USB cable
- Run `cp build/test_nvic.uf2 /media/$USER/RP2350` (flashes the board with the .uf2 file)
    - Alternatively, run `build/_deps/picotool/picotool load -u -v -x build/test_nvic.elf`
