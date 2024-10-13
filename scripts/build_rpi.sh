#!/bin/bash
set -e	# exit on error

# Adjust path to match your environment
export ELK_SDK_DIR=/opt/sdk/elk-glibc-x86_64-elkpi-audio-os-image-cortexa72-raspberrypi4-64-toolchain-1.0.0

cd targets/rpi4
mkdir build &> /dev/null || true
cd build
source ${ELK_SDK_DIR}/environment-setup-cortexa72-elk-linux
cmake -DCMAKE_BUILD_TYPE=Release ..
make
