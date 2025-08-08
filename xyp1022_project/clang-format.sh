#!/bin/bash

ABSOLUTE_PATH=.
SRC_ROOT=$ABSOLUTE_PATH

CLANG_FORMAT=D:/MDK-ARM/Keil_v5/ARM/ARMCLANG/bin/clang-format.exe

find "$SRC_ROOT" -type f \( \
  -name "*.cpp" -o \
  -name "*.cc" -o \
  -name "*.h" -o \
  -name "*.c" -o \
  -name "*.hpp" \
\) -not -path "*/build/*" \
   -not -path "*/thirdparty/*" \
   -not -path "*/board/XYP1022.h" \
   -not -path "*/CFG/*" \
   -not -path "*/ThirdLib/*" \
   -not -path "*/utilities/*" \
   -not -path "*/libs/ff15/*" \
   -not -path "*/libs/coremark/*" \
   -not -path "*/projects/CW/*" \
   -not -path "*/projects/FreeRTOSConfig.h" \
   -not -path "*/.git/*" | while read f; do $CLANG_FORMAT --verbose -i "$f"
done

