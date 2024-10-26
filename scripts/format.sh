#!/bin/bash
set -e	# exit on error

# clang-format
srcdirs=$(find . -type d -name src -not -path "*/mcuxsdk/*" -not -path "*/CMSIS-DSP/*" -not -path "*/build/*")
for dir in ${srcdirs}; do
	find ${dir} -type f \( -iname \*.c -o -iname \*.h \) -exec clang-format -i {} \;
done

# convert line endings to unix
dos2unix results/* &> /dev/null
