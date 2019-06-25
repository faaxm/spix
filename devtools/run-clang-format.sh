#!/bin/sh
find . | grep -v -E "/(build|dep-libs)/.*" | grep -E "\\.(cpp|h)$" | xargs -L1 clang-format -i
