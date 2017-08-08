#!/bin/bash

make
../../Compiler_ExtensionBoard/bin/arm-none-eabi-gdb build/esho1environment.elf --command=gdb_script.sh

