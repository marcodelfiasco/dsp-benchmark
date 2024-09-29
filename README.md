This repository contains the code for the ADC24 talk "Crunching same numbers on different architectures."

# Targets

## ADSP-21569
### Hardware setup
TODO

### Software setup
TODO

### Build
```
cd targets/adsp-21569-ezkit
mkdir build
cd build
cmake -DCMAKE_SYSTEM_NAME=ADSP -DCMAKE_SYSTEM_PROCESSOR=21569 ..
make
```

### Run
TODO


## MIMXRT1170-EVKB
### Hardware setup
TODO

### Software setup
1. Download the MCUXpresso SDK to `targets/mimxrt1170-evkb` using `west`. It will take a while.
```
cd targets/mimxrt1170-evkb
west init -m https://github.com/NXPmicro/mcux-sdk --mr MCUX_2.16.000 mcuxsdk
cd mcuxsdk
west update
```

2. Download the [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) for AArch32 bare-metal target
and unzip to a folder on your host PC (usually `/opt`). We used `arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi`.

### Build
```
cd targets/mimxrt1170-evkb
mkdir build
cd build
export ARMGCC_DIR=/opt/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi    # Change this to your toolchain directory
cmake -DCMAKE_BUILD_TYPE=release -DCMAKE_TOOLCHAIN_FILE="../mcuxsdk/core/tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" ..
make
```

### Run
TODO


## Raspberry Pi 4
On this target we will use [ELK Audio OS](https://www.elk.audio/).

### Hardware setup
You can follow the ELK official documentation on https://elk-audio.github.io/elk-docs/html/intro/getting_started_with_raspberry.html.

### Software setup
You need to install the ELK Audio OS 1.0.0 SDK first.
Get it from https://github.com/elk-audio/elkpi-sdk/releases/download/1.0.0/elk-glibc-x86_64-elkpi-audio-os-image-cortexa72-raspberrypi4-64-toolchain-1.0.0.sh.
Run the script and install the SDK on your preferred path. We will refer to this path as `ELK_SDK_INSTALL_PATH` in the build instructions.

### Build
```
cd targets/rpi4
mkdir build
cd build
source ELK_SDK_INSTALL_PATH/environment-setup-cortexa72-elk-linux
cmake ..
make
```

### Run
TODO
