#!/bin/bash
set -e	# exit on error

# Adjust path to match your environment
export ADSP_ROOT=/opt/analog/cces/2.12.1
export LD_LIBRARY_PATH=${ADSP_ROOT}/System:${LD_LIBRARY_PATH}

cd targets/adsp-21569-ezkit
mkdir build &> /dev/null || true
cd build
cmake -DCMAKE_SYSTEM_NAME=ADSP -DCMAKE_SYSTEM_PROCESSOR=21569 ..
make -j${nproc}
