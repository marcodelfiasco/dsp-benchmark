#!/bin/bash
set -e	# exit on error

for target in adsp imxrt rpi; do
	scripts/build_${target}.sh
done
