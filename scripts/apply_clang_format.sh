#!/bin/bash
set -e	# exit on error

srcdirs=$(find . -type d -name src -not -path "*/mcuxsdk/*" -not -path "*/build/*")
for dir in ${srcdirs}; do
	find ${dir} -type f \( -iname \*.c -o -iname \*.h \) -exec clang-format -i {} \;
done
