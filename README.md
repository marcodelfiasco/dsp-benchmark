This repository contains the code for the ADC24 talk "Crunching same numbers on different architectures."

# Targets

## ADSP-21569
### Hardware setup
If you need to use test points you need to make some soldering on the EZ-Kit board. Test points are optional.

Test point reference:
* TP0: FLG2 - PB_03 -> LED7 (yellow)
* TP1: FLG3 - PB_02

See EZ-Kit board schematic for more details.

### Software setup
You should have a licensed copy of [Crosscore Embedded Studio](https://www.analog.com/en/resources/evaluation-hardware-and-software/software/adswt-cces.html) installed on your system.
For Linux you need the "SHARC and Blackfin Linux Command-Line Tools" add-in installed. Please refer to the official CCES documentation for more details.

### Build
```
cd targets/adsp-21569-ezkit
mkdir build
cd build
cmake -DCMAKE_SYSTEM_NAME=ADSP -DCMAKE_SYSTEM_PROCESSOR=21569 ..
make
```

or just use scripts in the `scripts` directory.

#### Caveats for WSL2
If things do not work from WSL then you can try to set the following environment variables:
```
export ADSP_ROOT=<your-cces-install-path>
export LD_LIBRARY_PATH=<your-cces-install-path>/System:${LD_LIBRARY_PATH}
```

It seems that the SHARC compiler running from WSL is not able to access mountpoints on `/mnt/`, so working under your local WSL home directory is recommended.

### Run
You can load the `.dxe` using CCES and run it from the IDE.


## MIMXRT1170-EVKB
### Hardware setup
If you need to use test points you need to make some soldering on the EVKB board. Test points are optional.

Test point reference:
* TP0: Green LED
* TP1: Red LED

See EVKB board schematic for more details.

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

or just use scripts in the `scripts` directory.

### Run
You can load the `.elf` using MCUXpresso and run it from the IDE.


## Raspberry Pi 4
On this target we will use [ELK Audio OS](https://www.elk.audio/).

The application runs on a realtime EVL thread with maximum priority.
This will minimize disturbance from other processes.

### Board setup
You can follow the ELK official documentation on https://elk-audio.github.io/elk-docs/html/intro/getting_started_with_raspberry.html.
After the official ELK setup is done we need to:
1. Disable automatic run of sushi.
2. Set the CPU frequency to 1.5GHz.

#### Disable sushi
Login to the board using ssh and run `sudo systemctl disable sushi`.

#### Set CPU frequency to 1.5GHz
WARNING: Running at 1.5GHz normally requires an heatsink. Do this at your own risk.

Login to the board using ssh and:
* run `sudo mount /dev/mmcblk0p1 /mnt/`
* run `sudo vim /mnt/config.txt`
* change `arm_freq=1200` to `arm_freq=1500`
* save and exit
* run `sync && sudo reboot`

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
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

or just use scripts in the `scripts` directory.

### Run
Execute `benchmark-rpi4` application.

# Plots
To generate plots run
```
python scripts/plot.py
```
