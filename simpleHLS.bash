#!/bin/bash
## Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
## date   Dec, 2016
## @brief  The c-ll-verilog tool script, usage: simpleHLS.bash <filename.c>
##
cfile=$1
file="${cfile%.*}"
./parser/parser "$file".c 2>&1 | opt -mem2reg | llvm-dis &> "$file".ll
cat "$file".ll | opt -load ./codegen/verilog.so -verilog -o "$file".v &> "$file".v