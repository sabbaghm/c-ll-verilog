#ifndef VERILOG_FUNCTION_H
#define VERILOG_FUNCTION_H


#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "Argument.h"

#include <unordered_map>


namespace verilog
{

// Forward declarations
class CodeBlock;
class Wire;
class Register;


class Module
{
	// Table of code blocks, indexed by name
	std::unordered_map<std::string, CodeBlock *> code_block_table;

	// List of code blocks, preseving the LLVM order
	std::list<CodeBlock *> code_blocks;

	// Entry and exit code blocks
	CodeBlock *entry_code_block;
	CodeBlock *exit_code_block;

	// Symbol table, associating LLVM variable names to VERILOG wires
	std::unordered_map<std::string, Wire *> wire_symbol_table;
	// Symbol table, associating LLVM variable names to VERILOG registers
	std::unordered_map<std::string, Register *> register_symbol_table;

public:

	// Constructor. It creates an entry and exit code blocks for the
	// function.
	Module();

	// Add a code block
	void addCodeBlock(CodeBlock *code_block);

	// Get code block given its label, or nullptr if it doesn't exist.
	CodeBlock *getCodeBlock(const std::string &name);

	// Return entry code block
	CodeBlock *getEntryCodeBlock() { return entry_code_block; }

	// Return exit code block
	CodeBlock *getExitCodeBlock() { return exit_code_block; }

	// Add symbol to symbol table
	void WireAddSymbol(const std::string &name, Wire *wire)
	{
		wire_symbol_table[name] = wire;
	}

	// Add symbol to symbol table
	void RegisterAddSymbol(const std::string &name, Register *reg)
	{
		register_symbol_table[name] = reg;
	}

	// Lookup symbol table, and produce error if symbol not found.
	Wire *wirelookupSymbol(const std::string &name);
	
	// Lookup symbol table, and produce error if symbol not found.
	Register *registerlookupSymbol(const std::string &name);

	// If the given value is an LLVM variable, return a new wire
	// associated with the symbol as read from the symbol table. If the
	// value is an LLVM constant, emit an 'li' instruction in the given
	// code block, and return the wire that was assigned the constant.
	Wire *translateLLVMValueWire(CodeBlock *code_block,
			llvm::Value *llvm_value,
			Wire::Direction direction
			);

	// If the given value is an LLVM variable, return a new register
	// associated with the symbol as read from the symbol table. If the
	// value is an LLVM constant, emit an 'li' instruction in the given
	// code block, and return the register that was assigned the constant.
	Register *translateLLVMValueRegister(CodeBlock *code_block,
			llvm::Value *llvm_value,
			Register::Direction direction,
			bool seqReg = false);

	// Print it
	void dump();
};

}


#endif

