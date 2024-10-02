#!/bin/bash
set -e	# exit on error

# Adjust path to match your environment
export ARMGCC_DIR=/opt/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi

cd targets/mimxrt1170-evkb
mkdir build &> /dev/null || true
cd build
cmake -DCMAKE_BUILD_TYPE=release -DCMAKE_TOOLCHAIN_FILE="../mcuxsdk/core/tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" ..
make -j${nproc}
