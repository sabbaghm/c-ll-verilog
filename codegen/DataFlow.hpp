/**
 * @file   DataFlow.hpp
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  Dataflow class contains acceptable verilog statements.
 */

/**
 * Update: Support New Pass Manager (tested on LLVM 17/18) and migrate to CPP
 * @author Mohamed Bouaziz (mohamed.bouaziz@kaust.edu.sa), KAUST
 * @date   Oct, 2023
*/

#ifndef VERILOG_INSTRUCTION_H
#define VERILOG_INSTRUCTION_H

#include <iostream>
#include <list>

#include "Argument.hpp"


namespace verilog
{

class DataFlow
{
public:

	// Instruction kinds
	enum Opcode
	{
		OpcodeInvalid = 0,
		
		OpcodePortListStart,
		OpcodeInputPortDefine,
		OpcodeOutputPortDefine,
		OpcodePortListEnd,
		OpcodeSignalDefine,
		OpcodeAssign,
		OpcodeAdd,
		OpcodeSub,
		OpcodeLshr,
		OpcodeXor,
		OpcodeAnd,
		OpcodeOr,
		OpcodeEq,
		OpcodeSgt,
		OpcodeEndModule,

		OpcodeMax
	};

private:
	
	// Instruction names
	static std::string names[OpcodeMax];

	// Opcode
	Opcode opcode;

	// List of arguments
	std::list<Argument *> arguments;
	
public:

	// Constructor
	DataFlow(Opcode opcode) : opcode(opcode)
	{
	}

	// Add an argument
	void addArgument(Argument *argument)
	{
		arguments.push_back(argument);
	}


	// Print dataflow
	void dump();
};


}

#endif

