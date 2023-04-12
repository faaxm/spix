#!/bin/sh
find . \( -iname "*.cpp" -or -iname "*.h" \) -not \( -ipath "*/build/*" -or -ipath "./dep-libs/*" \) | xargs -L1 clang-format-11 -i
