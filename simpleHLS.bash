#!/bin/bash
cfile=$1
file="${cfile%.*}"
./parser/parser "$file".c 2>&1 | opt -mem2reg | llvm-dis &> "$file".ll
cat "$file".ll | opt -load ./codegen/verilog.so -verilog -o "$file".v &> "$file".v