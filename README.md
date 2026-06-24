# Raspberry Pi Pico 2 NVIC example

This example demonstrates how to use the NVIC (nested vectored interrupt controller) in the ARM Cortex-M microcontroller processors, such as the Cortex-M33 found in the dual-core RP2350 SoC used by the Raspberry Pi Pico 2. It essentially performs hardware-accelerated interrupt servicing by fetching the ISR address directly from a hardware vector table. This differs from the GIC (generic interrupt controller) found in Cortex-A microprocessors, where a single generic interrupt vector receives all interrupts, and software is expected to read GIC registers to find the specific interrupt ID and then the ISR. The GIC is better suited for multi-core environments.

Instructions:

- Clone https://github.com/raspberrypi/pico-sdk
- In `~/.bashrc` add `export PICO_SDK_PATH=/path/to/pico-sdk`
- In the `pico-sdk` folder run `git submodule update --init`
- Run `sudo apt install build-essential cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib`
- Clone https://github.com/raspberrypi/picotool
- Run `sudo apt install pkg-config libusb-1.0-0-dev`
- In `~/.bashrc` add `export PKG_CONFIG_PATH="/usr/lib/x86_64-linux-gnu/pkgconfig/"`
- In the `picotool` folder run 
    ```
    cmake -S . -B build
    cmake --build build
    sudo cmake --install build
    sudo cp udev/60-picotool.rules /etc/udev/rules.d/
    sudo udevadm control --reload
    ```

To generate Makefiles: `cmake -DPICO_BOARD=pico2 -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/arm-none-eabi-gcc -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/arm-none-eabi-g++ -S . -B build`

To build application: `cmake --build build`

To run on board: 

- Unplug USB cable from board
- Hold down BOOTSEL button while plugging in USB cable
- Run `cp build/hello_world.uf2 /media/$USER/RP2350` (flashes the board with the .uf2 file)
    - Alternatively, run `picotool load -u -v -x build/hello_world.elf`
