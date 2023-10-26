/**
 * @file   Module.cpp
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  This file defines the operations on verilog modules.
 */

/**
 * Update: Support New Pass Manager (tested on LLVM 17/18) and migrate to CPP
 * @author Mohamed Bouaziz (mohamed.bouaziz@kaust.edu.sa), KAUST
 * @date   Oct, 2023
*/

#include "CodeBlock.hpp"
#include "Module.hpp"
#include "DataFlow.hpp"


namespace verilog
{


Module::Module()
{
	entry_code_block = new CodeBlock("entry");
//	for(auto& functionArgs : 
	exit_code_block = new CodeBlock("exit");
}

void Module::addCodeBlock(CodeBlock *code_block)
{
	code_block_table[code_block->getName()] = code_block;
	code_blocks.push_back(code_block);
}
	

CodeBlock *Module::getCodeBlock(const std::string &name)
{
	auto it = code_block_table.find(name);
	return it == code_block_table.end() ? nullptr : it->second;
}


Wire *Module::wirelookupSymbol(const std::string &name)
{
	auto it = wire_symbol_table.find(name);
	if (it == wire_symbol_table.end())
	{
		std::cerr << "Error: symbol '" << name << "' not found\n";
		exit(1);
	}
	return it->second;
}

Register *Module::registerlookupSymbol(const std::string &name)
{
	auto it = register_symbol_table.find(name);
	if (it == register_symbol_table.end())
	{
		std::cerr << "Error: symbol '" << name << "' not found\n";
		exit(1);
	}
	return it->second;
}

Wire *Module::translateLLVMValueWire(CodeBlock *code_block,
		llvm::Value *llvm_value,
		Wire::Direction direction)
{
	if (llvm_value->getValueID() == llvm::Value::ConstantIntVal)
	{

                // Emit 'assign dest = constant;'
                DataFlow *dataflow = new DataFlow(
                DataFlow::OpcodeAssign);
                                                               
		// First Argument
		Wire *dest = Wire::newWire(Wire::DirectionOutput);

		// Second argument
		llvm::ConstantInt *llvm_constant = llvm::cast<llvm::ConstantInt>
				(llvm_value);
		Constant *constant = new Constant(llvm_constant->getSExtValue());
		verilog::DataFlow *verilog_signaldef = new verilog::DataFlow(verilog::DataFlow::OpcodeSignalDefine);

			llvm::Type *type = llvm_constant->getType();	
			if (type->isIntegerTy(32))
			{
				dest->setWidth(32);	
			}
			else if (type->isIntegerTy(16))
			{
				dest->setWidth(16);	
			}
			else if (type->isIntegerTy(8))
			{
				dest->setWidth(8);	
			}
			else if (type->isIntegerTy(1))
			{
				dest->setWidth(1);	
			}
			else
			{
				std::cerr << "Unsupported type in temporay registers\n";
				exit(1);
			}
                code_block->addDataFlow(verilog_signaldef);
                verilog_signaldef->addArgument(dest);

                code_block->addDataFlow(dataflow);
		dataflow->addArgument(dest);
		dataflow->addArgument(constant);


		// Return copy of created register
		return new Wire(dest->getName(), direction);
	}
	else
	{
		Wire *wire = wirelookupSymbol(llvm_value->getName().str());
		return new Wire(wire->getName(), direction);
	}
}

Register *Module::translateLLVMValueRegister(CodeBlock *code_block,
		llvm::Value *llvm_value,
		Register::Direction direction,
		bool seqReg)
{
	if (llvm_value->getValueID() == llvm::Value::ConstantIntVal)
	{
                // Emit 'assign dest = constant;'
                DataFlow *dataflow = new DataFlow(
                DataFlow::OpcodeAssign);
                code_block->addDataFlow(dataflow);

		// First Argument
		Register *dest = Register::newRegister(Register::DirectionOutput);
		dataflow->addArgument(dest);

		// Second argument
		llvm::ConstantInt *llvm_constant = llvm::cast<llvm::ConstantInt>
				(llvm_value);
		Constant *constant = new Constant(llvm_constant->getSExtValue());
		dataflow->addArgument(constant);

		// Return copy of created register
		return new Register(dest->getName(), direction, seqReg);
	}
	else
	{
		Register *reg = registerlookupSymbol(llvm_value->getName().str());
		return new Register(reg->getName(), direction, seqReg);
	}
}
	

void Module::dump()
{
	// Print all basic blocks
	for (CodeBlock *code_block : code_blocks)
		code_block->dump();
}

}

