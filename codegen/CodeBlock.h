/**
 * @file   CodeBlock.h
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  CodeBlock class contains interfaces for operating on a block of verilog statements.
 */

#ifndef VERILOG_BASIC_BLOCK_H
#define VERILOG_BASIC_BLOCK_H

#include <iostream>
#include <list>
#include <cassert>


namespace verilog
{

// Forward declarations
class DataFlow;

class CodeBlock
{
	// Label name
	std::string name;

	// List of dataFlows
	std::list<DataFlow *> dataflows;

        // List of successors and predecessors
        std::list<CodeBlock *> succ;
        std::list<CodeBlock *> pred;
                 
public:

	// Constructor
	CodeBlock(const std::string &name) : name(name)
	{
	}

	// Add dataFlow to code block
	void addDataFlow(DataFlow *dataflow)
	{
		dataflows.push_back(dataflow);
	}


        // Add a code block B2 to the list of successors of the current code
        // block B1. Also, add B1 to the list of predecessors of B2.
        void addSuccessor(CodeBlock *code_block)
        {
        	assert(this != code_block);
                succ.push_back(code_block);
                code_block->pred.push_back(this);
        }                                                                                         


	// Return label name
	const std::string &getName() { return name; }

	// Print code block
	void dump();
};


}

#endif

