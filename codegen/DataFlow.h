#ifndef VERILOG_INSTRUCTION_H
#define VERILOG_INSTRUCTION_H

#include <iostream>
#include <list>

#include "Argument.h"


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

