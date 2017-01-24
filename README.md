# c-ll-verilog
An LLVM based mini-C to Verilog High-level Synthesis tool
## Overview
In this project, we are proposing a limited LLVM based mini-C to Verilog high-level synthesis (HLS)
tool. HLS is a trending topic in both the academia and industry which aims to reduce the complexity of
hardware design by automatically generating hardware description language (HDL) code from a higher
level language such as C. An important different between programming languages like C and HDLs is
that, C and similar languages are meant to be used on a predefined architectures, which usually are
conventional von neumann architectures, while HDL itself (in the domain of FPGA programming) is
meant to model and design the underlying architecture or hardware. There are different HLS tools which
their generated HDL code can be implemented on the latest field programmable gate array (FPGA) or
system-on-chips. However, due to the inherent syntax and symantec differences in general purpose high
level languages such as C and HDLs and the complexity of this conversion, current HLS tools cannot yet
satisfy all of the performance, area, and power requirements of the complex systems. Therefore, in the
view of the author these tools are either used in a highly annotated manner (for e.g. using PRAGMAS or
compiler directives) for a specific FPGA device and application or used for experimental as well as
educational purposes. In this project we investigated the HLS process using an [LLVM](llvm.org) based framework,
and by compiling a subset of branchless C statements to dataflow modeling [Verilog](www.verilog.com) code.
## What's inside and how it works?
The overall goal of this project was to convert a C function to a Verilog module, with the following
features:
	* C function arguments are used as Verilog module input ports (no pointer, array, and struct
support)
	* C function return value are used as Verilog module output port
	* Only a subset of branchless C statements is supported
	* Only a subset of arithmetic and logical operations are supported (addition, subtraction, signed greater than, equality, logical shift right, bitwise XOR, bitwise AND, and bitwise OR)
	* Only combinational logic with data modeling Verilog style is supported (no registers and clock)
In the first step the latest LLVM parser is extended to support non void returns from functions. For that
the scanner.l and parser.y files are modified as showed in the README file included in the tar ball. After
that, the code generation part is built upon the MIPS code generator LLVM pass. To create the simple
mini-C-to-Verilog code generator (as `verilog` LLVM `opt` pass) in a linux machine with proper
installation of LLVM framework, extract the tar ball and run make command in the main directory. To
run the HLS tool pass your c file to the simpleHLS.bash script as below (make sure [simpleHLS.bash](sabbaghm/master/scripts/simpleHLS.bash) is
executable): `./simpleHLS.bash <filename>.c`
You should see the generated Verilog file in the same directory as <filename>.v.
Note that after the parsing process mem2reg LLVM optimization is used to simplify the LLVM code
remove memory allocation and accesses as it is not supported by our simple HLS tool.
An example C code ([filter.c](sabbaghm/srcs/repo)),
```
bool adder(int in1, int in2, int threshold,
int mask)
{
	return ( ( ( (in1+in2) & mask ) >> 2 ) > threshold );
}
```
 and its conversion to Verilog code ([filter.v](sabbaghm/srcs/repo):

```
//entry:
module adder (
input wire [31:0] in1,
input wire [31:0] in2,
input wire [31:0] threshold,
input wire [31:0] mask,
output wire result
);
//L0:
wire [31:0] wire000;
assign wire000 = in1 + in2;
wire [31:0] wire001;
assign wire001 = wire000 & mask;
wire [31:0] wire003;
assign wire003 = 2;
wire [31:0] wire002;
assign wire002 = wire001 >> wire003;
wire wire004;
assign wire004 = wire002 > threshold;
assign result = wire004;
//exit:
endmodule

```

