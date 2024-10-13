#!/bin/bash
set -e	# exit on error

for target in adsp imxrt rpi; do
    scripts/clean_${target}.sh
done
