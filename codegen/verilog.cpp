/**
 * @file   verilog.cpp
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  This class defines the verilog optimization pass.
 */

/**
 * Update: Support New Pass Manager (tested on LLVM 17/18) and migrate to CPP
 * @author Mohamed Bouaziz (mohamed.bouaziz@kaust.edu.sa), KAUST
 * @date   Oct, 2023
*/

#include <iostream>

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
// #include "llvm/Support/CFG.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include "CodeBlock.hpp"
#include "Module.hpp"
#include "DataFlow.hpp"

using namespace llvm;

namespace
{

	struct VERILOGPass : public PassInfoMixin<VERILOGPass>
	{

		// Identifier
		// static char ID;

		// verilog module
		verilog::Module *verilog_module;

		verilog::Wire *verilog_result = new verilog::Wire("result", verilog::Wire::DirectionOutput);

		std::string widthStr = "";
		// Constructor. The first argument of the parent constructor is
		// a unique pass identifier.
		// VERILOGPass() : FunctionPass(ID) {}

		// // Virtual function overridden to implement the pass functionality.
		// virtual bool runOnFunction(Function &llvm_function) override;

		PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);

		void TranslateBasicBlock(BasicBlock *llvm_basic_block,
								 verilog::CodeBlock *verilog_code_block);

		void TranslateInstruction(Instruction *llvm_instruction,
								  verilog::CodeBlock *verilog_code_block);
	};

	// bool VERILOGPass::runOnFunction(Function &llvm_function)
	PreservedAnalyses VERILOGPass::run(Function &F, FunctionAnalysisManager &FAM)
	{
		// Create VERILOG module
		verilog_module = new verilog::Module();

		// Add 'entry' code block
		verilog::CodeBlock *verilog_entry_code_block = verilog_module->getEntryCodeBlock();

		// Emit "module <llvmfuncname> (ports);"
		verilog::DataFlow *verilog_portliststart = new verilog::DataFlow(verilog::DataFlow::OpcodePortListStart);
		verilog::Wire *verilog_wire = new verilog::Wire(F.getName().str(), verilog::Wire::DirectionInvalid);
		verilog_portliststart->addArgument(verilog_wire);
		verilog_entry_code_block->addDataFlow(verilog_portliststart);

		for (auto &functionArgument : F.args())
		{
			// Wire support, add register support later
			verilog::DataFlow *verilog_dataflow = new verilog::DataFlow(verilog::DataFlow::OpcodeInputPortDefine);
			verilog::Wire *verilog_wire = new verilog::Wire(functionArgument.getName().str(), verilog::Wire::DirectionInput);
			Type *type = functionArgument.getType();
			if (type->isIntegerTy(32))
			{
				verilog_wire->setWidth(32);
			}
			else if (type->isIntegerTy(16))
			{
				verilog_wire->setWidth(16);
			}
			else if (type->isIntegerTy(8))
			{
				verilog_wire->setWidth(8);
			}
			else if (type->isIntegerTy(1))
			{
				verilog_wire->setWidth(1);
			}
			else
			{
				std::cerr << "Unsupported type in function arguments\n";
				exit(1);
			}
			verilog_module->WireAddSymbol(functionArgument.getName().str(), verilog_wire);
			verilog_dataflow->addArgument(verilog_wire);
			verilog_entry_code_block->addDataFlow(verilog_dataflow);
		}

		verilog::DataFlow *verilog_dataflow_out = new verilog::DataFlow(verilog::DataFlow::OpcodeOutputPortDefine);
		Type *type = F.getReturnType();
		if (type->isIntegerTy(32))
		{
			verilog_result->setWidth(32);
		}
		else if (type->isIntegerTy(16))
		{
			verilog_result->setWidth(16);
		}
		else if (type->isIntegerTy(8))
		{
			verilog_result->setWidth(8);
		}
		else if (type->isIntegerTy(1))
		{
			verilog_result->setWidth(1);
		}
		else
		{
			std::cerr << "Unsupported type in function arguments\n";
			exit(1);
		}
		verilog_dataflow_out->addArgument(verilog_result);
		verilog_entry_code_block->addDataFlow(verilog_dataflow_out);

		// wrap up the module port list ");"
		verilog::DataFlow *verilog_portlistend = new verilog::DataFlow(verilog::DataFlow::OpcodePortListEnd);
		verilog_entry_code_block->addDataFlow(verilog_portlistend);

		verilog_module->addCodeBlock(verilog_entry_code_block);

		// Create all basic blocks
		for (Function::iterator llvm_basic_block = F.begin(),
								   e = F.end();
			 llvm_basic_block != e;
			 ++llvm_basic_block)
		{
			verilog::CodeBlock *verilog_code_block =
				new verilog::CodeBlock(llvm_basic_block->getName().str());
			verilog_module->addCodeBlock(verilog_code_block);
		}

		// Set first code block as successor of module's entry basic block
		BasicBlock *llvm_first_basic_block = &F.getEntryBlock();
		verilog::CodeBlock *verilog_first_code_block = verilog_module->getCodeBlock(
			llvm_first_basic_block->getName().str());
		verilog_entry_code_block->addSuccessor(verilog_first_code_block);

		// Code generation for code blocks
		for (auto &llvm_basic_block : F)
		{
			verilog::CodeBlock *verilog_code_block =
				verilog_module->getCodeBlock(
					(&llvm_basic_block)->getName().str());
			TranslateBasicBlock(&llvm_basic_block, verilog_code_block);
		}

		// Add 'exit' code block
		verilog::CodeBlock *verilog_exit_code_block = verilog_module->getExitCodeBlock();
		verilog_module->addCodeBlock(verilog_exit_code_block);

		// Emit 'endmodule' in 'exit' code block
		verilog::DataFlow *verilog_dataflow = new verilog::DataFlow(
			verilog::DataFlow::OpcodeEndModule);
		verilog_exit_code_block->addDataFlow(verilog_dataflow);

		// Print verilog module
		verilog_module->dump();

		// Function was not modified
		return PreservedAnalyses::all();
	}

	void VERILOGPass::TranslateBasicBlock(BasicBlock *llvm_basic_block,
										  verilog::CodeBlock *verilog_code_block)
	{
		// Traverse instructions
		for (auto &llvm_instruction : *llvm_basic_block)
		{
			TranslateInstruction(&llvm_instruction, verilog_code_block);
		}
	}

	void VERILOGPass::TranslateInstruction(Instruction *llvm_instruction,
										   verilog::CodeBlock *verilog_code_block)
	{
		switch (llvm_instruction->getOpcode())
		{
		case Instruction::Alloca:
		{
			// Get alloca instruction
			AllocaInst *alloca_inst = cast<AllocaInst>(llvm_instruction);

			// Check type
			Type *type = alloca_inst->getAllocatedType();
			if (!type->isIntegerTy(32))
			{
				std::cerr << "Unsupported type in alloca\n";
				exit(1);
			}

			// Get 'entry' and 'exit' code blocks
			verilog::DataFlow *verilog_dataflow;
			verilog::CodeBlock *verilog_entry_code_block = verilog_module->getEntryCodeBlock();
			verilog::CodeBlock *verilog_exit_code_block = verilog_module->getExitCodeBlock();

			// Emit 'addi $sp, $sp, -4' in '__entry' basic block
			//		mips_instruction = new mips::Instruction(
			//				mips::Instruction::OpcodeAddi);
			//		mips_instruction->addArgument(new mips::Register(29,
			//				mips::Register::DirectionOutput));
			//		mips_instruction->addArgument(new mips::Register(29,
			//				mips::Register::DirectionInput));
			//		mips_instruction->addArgument(new mips::Constant(-4));
			//		mips_entry_basic_block->addInstruction(mips_instruction);

			// Emit 'move $temp, $sp' in 'entry' basic block.
			// Store '$temp' in symbol table
			verilog_dataflow = new verilog::DataFlow(verilog::DataFlow::OpcodeSignalDefine);
			// Currently only supports wire signals, can be extended to support registers
			// and sequential logic
			verilog::Wire *dest = verilog::Wire::newWire(
				verilog::Wire::DirectionLhs);
			verilog_dataflow->addArgument(dest);
			verilog_dataflow->addArgument(new verilog::Wire("wire0",
															verilog::Wire::DirectionLhs));
			verilog_entry_code_block->addDataFlow(verilog_dataflow);
			verilog_module->WireAddSymbol(llvm_instruction->getName().str(), dest);

			break;
		}

		case Instruction::Add:
		{
			// Destination wire
			verilog::Wire *verilog_dest = verilog::Wire::newWire(
				verilog::Wire::DirectionLhs);
			verilog_module->WireAddSymbol(llvm_instruction->getName().str(), verilog_dest);

			// First operand
			Value *llvm_src1 = llvm_instruction->getOperand(0);
			verilog::Wire *verilog_src1 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src1,
				verilog::Wire::DirectionRhs);

			// Second operand
			Value *llvm_src2 = llvm_instruction->getOperand(1);
			verilog::Wire *verilog_src2 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src2,
				verilog::Wire::DirectionRhs);

			verilog::DataFlow *verilog_signaldef = new verilog::DataFlow(verilog::DataFlow::OpcodeSignalDefine);
			Type *type = llvm_instruction->getType();
			if (type->isIntegerTy(32))
			{
				verilog_dest->setWidth(32);
			}
			else if (type->isIntegerTy(16))
			{
				verilog_dest->setWidth(16);
			}
			else if (type->isIntegerTy(8))
			{
				verilog_dest->setWidth(8);
			}
			else if (type->isIntegerTy(1))
			{
				verilog_dest->setWidth(1);
			}
			else
			{
				std::cerr << "Unsupported type in temporay registers\n";
				exit(1);
			}
			verilog_code_block->addDataFlow(verilog_signaldef);
			verilog_signaldef->addArgument(verilog_dest);

			// Emit '>' operator
			verilog::DataFlow *verilog_dataflow =
				new verilog::DataFlow(
					verilog::DataFlow::OpcodeAdd);
			verilog_code_block->addDataFlow(verilog_dataflow);
			verilog_dataflow->addArgument(verilog_dest);

			// src1 and src2
			verilog_dataflow->addArgument(verilog_src1);
			verilog_dataflow->addArgument(verilog_src2);
			break;
		}

		case Instruction::Sub:
		{
			// Destination wire
			verilog::Wire *verilog_dest = verilog::Wire::newWire(
				verilog::Wire::DirectionLhs);
			verilog_module->WireAddSymbol(llvm_instruction->getName().str(), verilog_dest);

			// First operand
			Value *llvm_src1 = llvm_instruction->getOperand(0);
			verilog::Wire *verilog_src1 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src1,
				verilog::Wire::DirectionRhs);

			// Second operand
			Value *llvm_src2 = llvm_instruction->getOperand(1);
			verilog::Wire *verilog_src2 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src2,
				verilog::Wire::DirectionRhs);

			verilog::DataFlow *verilog_signaldef = new verilog::DataFlow(verilog::DataFlow::OpcodeSignalDefine);
			Type *type = llvm_instruction->getType();
			if (type->isIntegerTy(32))
			{
				verilog_dest->setWidth(32);
			}
			else if (type->isIntegerTy(16))
			{
				verilog_dest->setWidth(16);
			}
			else if (type->isIntegerTy(8))
			{
				verilog_dest->setWidth(8);
			}
			else if (type->isIntegerTy(1))
			{
				verilog_dest->setWidth(1);
			}
			else
			{
				std::cerr << "Unsupported type in temporay registers\n";
				exit(1);
			}
			verilog_code_block->addDataFlow(verilog_signaldef);
			verilog_signaldef->addArgument(verilog_dest);

			// Emit '>' operator
			verilog::DataFlow *verilog_dataflow =
				new verilog::DataFlow(
					verilog::DataFlow::OpcodeSub);
			verilog_code_block->addDataFlow(verilog_dataflow);
			verilog_dataflow->addArgument(verilog_dest);

			// src1 and src2
			verilog_dataflow->addArgument(verilog_src1);
			verilog_dataflow->addArgument(verilog_src2);
			break;
		}
		case Instruction::LShr:
		{
			// Destination wire
			verilog::Wire *verilog_dest = verilog::Wire::newWire(
				verilog::Wire::DirectionLhs);
			verilog_module->WireAddSymbol(llvm_instruction->getName().str(), verilog_dest);

			// First operand
			Value *llvm_src1 = llvm_instruction->getOperand(0);
			verilog::Wire *verilog_src1 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src1,
				verilog::Wire::DirectionRhs);

			// Second operand
			Value *llvm_src2 = llvm_instruction->getOperand(1);
			verilog::Wire *verilog_src2 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src2,
				verilog::Wire::DirectionRhs);

			verilog::DataFlow *verilog_signaldef = new verilog::DataFlow(verilog::DataFlow::OpcodeSignalDefine);
			Type *type = llvm_instruction->getType();
			if (type->isIntegerTy(32))
			{
				verilog_dest->setWidth(32);
			}
			else if (type->isIntegerTy(16))
			{
				verilog_dest->setWidth(16);
			}
			else if (type->isIntegerTy(8))
			{
				verilog_dest->setWidth(8);
			}
			else if (type->isIntegerTy(1))
			{
				verilog_dest->setWidth(1);
			}
			else
			{
				std::cerr << "Unsupported type in temporay registers\n";
				exit(1);
			}
			verilog_code_block->addDataFlow(verilog_signaldef);
			verilog_signaldef->addArgument(verilog_dest);

			// Emit '>' operator
			verilog::DataFlow *verilog_dataflow =
				new verilog::DataFlow(
					verilog::DataFlow::OpcodeLshr);
			verilog_code_block->addDataFlow(verilog_dataflow);
			verilog_dataflow->addArgument(verilog_dest);

			// src1 and src2
			verilog_dataflow->addArgument(verilog_src1);
			verilog_dataflow->addArgument(verilog_src2);
			break;
		}
		case Instruction::Xor:
		{
			// Destination wire
			verilog::Wire *verilog_dest = verilog::Wire::newWire(
				verilog::Wire::DirectionLhs);
			verilog_module->WireAddSymbol(llvm_instruction->getName().str(), verilog_dest);

			// First operand
			Value *llvm_src1 = llvm_instruction->getOperand(0);
			verilog::Wire *verilog_src1 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src1,
				verilog::Wire::DirectionRhs);

			// Second operand
			Value *llvm_src2 = llvm_instruction->getOperand(1);
			verilog::Wire *verilog_src2 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src2,
				verilog::Wire::DirectionRhs);

			verilog::DataFlow *verilog_signaldef = new verilog::DataFlow(verilog::DataFlow::OpcodeSignalDefine);
			Type *type = llvm_instruction->getType();
			if (type->isIntegerTy(32))
			{
				verilog_dest->setWidth(32);
			}
			else if (type->isIntegerTy(16))
			{
				verilog_dest->setWidth(16);
			}
			else if (type->isIntegerTy(8))
			{
				verilog_dest->setWidth(8);
			}
			else if (type->isIntegerTy(1))
			{
				verilog_dest->setWidth(1);
			}
			else
			{
				std::cerr << "Unsupported type in temporay registers\n";
				exit(1);
			}
			verilog_code_block->addDataFlow(verilog_signaldef);
			verilog_signaldef->addArgument(verilog_dest);

			// Emit '>' operator
			verilog::DataFlow *verilog_dataflow =
				new verilog::DataFlow(
					verilog::DataFlow::OpcodeXor);
			verilog_code_block->addDataFlow(verilog_dataflow);
			verilog_dataflow->addArgument(verilog_dest);

			// src1 and src2
			verilog_dataflow->addArgument(verilog_src1);
			verilog_dataflow->addArgument(verilog_src2);
			break;
		}
		case Instruction::And:
		{
			// Destination wire
			verilog::Wire *verilog_dest = verilog::Wire::newWire(
				verilog::Wire::DirectionLhs);
			verilog_module->WireAddSymbol(llvm_instruction->getName().str(), verilog_dest);

			// First operand
			Value *llvm_src1 = llvm_instruction->getOperand(0);
			verilog::Wire *verilog_src1 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src1,
				verilog::Wire::DirectionRhs);

			// Second operand
			Value *llvm_src2 = llvm_instruction->getOperand(1);
			verilog::Wire *verilog_src2 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src2,
				verilog::Wire::DirectionRhs);

			verilog::DataFlow *verilog_signaldef = new verilog::DataFlow(verilog::DataFlow::OpcodeSignalDefine);
			Type *type = llvm_instruction->getType();
			if (type->isIntegerTy(32))
			{
				verilog_dest->setWidth(32);
			}
			else if (type->isIntegerTy(16))
			{
				verilog_dest->setWidth(16);
			}
			else if (type->isIntegerTy(8))
			{
				verilog_dest->setWidth(8);
			}
			else if (type->isIntegerTy(1))
			{
				verilog_dest->setWidth(1);
			}
			else
			{
				std::cerr << "Unsupported type in temporay registers\n";
				exit(1);
			}
			verilog_code_block->addDataFlow(verilog_signaldef);
			verilog_signaldef->addArgument(verilog_dest);

			// Emit '>' operator
			verilog::DataFlow *verilog_dataflow =
				new verilog::DataFlow(
					verilog::DataFlow::OpcodeAnd);
			verilog_code_block->addDataFlow(verilog_dataflow);
			verilog_dataflow->addArgument(verilog_dest);

			// src1 and src2
			verilog_dataflow->addArgument(verilog_src1);
			verilog_dataflow->addArgument(verilog_src2);
			break;
		}
		case Instruction::Or:
		{
			// Destination wire
			verilog::Wire *verilog_dest = verilog::Wire::newWire(
				verilog::Wire::DirectionLhs);
			verilog_module->WireAddSymbol(llvm_instruction->getName().str(), verilog_dest);

			// First operand
			Value *llvm_src1 = llvm_instruction->getOperand(0);
			verilog::Wire *verilog_src1 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src1,
				verilog::Wire::DirectionRhs);

			// Second operand
			Value *llvm_src2 = llvm_instruction->getOperand(1);
			verilog::Wire *verilog_src2 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src2,
				verilog::Wire::DirectionRhs);

			verilog::DataFlow *verilog_signaldef = new verilog::DataFlow(verilog::DataFlow::OpcodeSignalDefine);
			Type *type = llvm_instruction->getType();
			if (type->isIntegerTy(32))
			{
				verilog_dest->setWidth(32);
			}
			else if (type->isIntegerTy(16))
			{
				verilog_dest->setWidth(16);
			}
			else if (type->isIntegerTy(8))
			{
				verilog_dest->setWidth(8);
			}
			else if (type->isIntegerTy(1))
			{
				verilog_dest->setWidth(1);
			}
			else
			{
				std::cerr << "Unsupported type in temporay registers\n";
				exit(1);
			}
			verilog_code_block->addDataFlow(verilog_signaldef);
			verilog_signaldef->addArgument(verilog_dest);

			// Emit '>' operator
			verilog::DataFlow *verilog_dataflow =
				new verilog::DataFlow(
					verilog::DataFlow::OpcodeOr);
			verilog_code_block->addDataFlow(verilog_dataflow);
			verilog_dataflow->addArgument(verilog_dest);

			// src1 and src2
			verilog_dataflow->addArgument(verilog_src1);
			verilog_dataflow->addArgument(verilog_src2);
			break;
		}
		case Instruction::ICmp:
		{
			// Destination wire
			verilog::Wire *verilog_dest = verilog::Wire::newWire(
				verilog::Wire::DirectionLhs);
			verilog_module->WireAddSymbol(llvm_instruction->getName().str(), verilog_dest);

			// First operand
			Value *llvm_src1 = llvm_instruction->getOperand(0);
			verilog::Wire *verilog_src1 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src1,
				verilog::Wire::DirectionRhs);

			// Second operand
			Value *llvm_src2 = llvm_instruction->getOperand(1);
			verilog::Wire *verilog_src2 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src2,
				verilog::Wire::DirectionRhs);

			// Operation depending on condition
			ICmpInst *icmp_instruction = cast<ICmpInst>(llvm_instruction);
			CmpInst::Predicate pred = icmp_instruction->getPredicate();
			switch (pred)
			{

			case CmpInst::ICMP_EQ:
			{

				verilog::DataFlow *verilog_signaldef = new verilog::DataFlow(verilog::DataFlow::OpcodeSignalDefine);
				Type *type = llvm_instruction->getType();
				if (type->isIntegerTy(32))
				{
					verilog_dest->setWidth(32);
				}
				else if (type->isIntegerTy(16))
				{
					verilog_dest->setWidth(16);
				}
				else if (type->isIntegerTy(8))
				{
					verilog_dest->setWidth(8);
				}
				else if (type->isIntegerTy(1))
				{
					verilog_dest->setWidth(1);
				}
				else
				{
					std::cerr << "Unsupported type in temporay registers\n";
					exit(1);
				}
				verilog_code_block->addDataFlow(verilog_signaldef);
				verilog_signaldef->addArgument(verilog_dest);

				// Emit '>' operator
				verilog::DataFlow *verilog_dataflow =
					new verilog::DataFlow(
						verilog::DataFlow::OpcodeEq);
				verilog_code_block->addDataFlow(verilog_dataflow);
				verilog_dataflow->addArgument(verilog_dest);

				// src1 and src2
				verilog_dataflow->addArgument(verilog_src1);
				verilog_dataflow->addArgument(verilog_src2);
				break;
			}
			case CmpInst::ICMP_SGT:
			{

				verilog::DataFlow *verilog_signaldef = new verilog::DataFlow(verilog::DataFlow::OpcodeSignalDefine);
				Type *type = llvm_instruction->getType();
				if (type->isIntegerTy(32))
				{
					verilog_dest->setWidth(32);
				}
				else if (type->isIntegerTy(16))
				{
					verilog_dest->setWidth(16);
				}
				else if (type->isIntegerTy(8))
				{
					verilog_dest->setWidth(8);
				}
				else if (type->isIntegerTy(1))
				{
					verilog_dest->setWidth(1);
				}
				else
				{
					std::cerr << "Unsupported type in temporay registers\n";
					exit(1);
				}
				verilog_code_block->addDataFlow(verilog_signaldef);
				verilog_signaldef->addArgument(verilog_dest);

				// Emit '>' operator
				verilog::DataFlow *verilog_dataflow =
					new verilog::DataFlow(
						verilog::DataFlow::OpcodeSgt);
				verilog_code_block->addDataFlow(verilog_dataflow);
				verilog_dataflow->addArgument(verilog_dest);

				// src1 and src2
				verilog_dataflow->addArgument(verilog_src1);
				verilog_dataflow->addArgument(verilog_src2);
				break;
			}

			default:

				std::cerr << "icmp predicate not supported\n";
				exit(1);
			}
			break;
		}

		case Instruction::Ret:
		{
			// Emit 'assign output = finalresult'
			verilog::DataFlow *dataflow = new verilog::DataFlow(
				verilog::DataFlow::OpcodeAssign);
			// return value
			//		verilog::Wire *verilog_dest = verilog::Wire::newWire(
			//				verilog::Wire::DirectionOutput);
			verilog_module->WireAddSymbol(llvm_instruction->getName().str(), verilog_result);

			dataflow->addArgument(verilog_result);

			// src operand
			Value *llvm_src1 = llvm_instruction->getOperand(0);
			verilog::Wire *verilog_src1 = verilog_module->translateLLVMValueWire(
				verilog_code_block,
				llvm_src1,
				verilog::Wire::DirectionInput);

			dataflow->addArgument(verilog_src1);

			verilog_code_block->addDataFlow(dataflow);

			// Set exit basic block as successor
			verilog_code_block->addSuccessor(verilog_module->getExitCodeBlock());
			break;
		}

		default:

			std::cerr << "Unsupported LLVM instruction: ";
			llvm_instruction->dump();
			std::cerr << '\n';
			exit(1);
		}
	}

}

// // Pass identifier
// char VERILOGPass::ID = 0;

// // Pass registration
// static RegisterPass<VERILOGPass> X("verilog",
// 									  "verilog code generation pass",
// 									  false,
// 									  false);

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "verilogPass", "v2",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "verilog-pass"){
            FPM.addPass(VERILOGPass());
            return true;
          }
          return false;
        }
      );
    }
  };
}