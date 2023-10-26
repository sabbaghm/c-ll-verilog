/**
 * @file   CodeBlock.cpp
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  This files dumps the code block name and its contents in a proper manner.
 */

/**
 * Update: Support New Pass Manager (tested on LLVM 17/18) and migrate to CPP
 * @author Mohamed Bouaziz (mohamed.bouaziz@kaust.edu.sa), KAUST
 * @date   Oct, 2023
*/

#include "CodeBlock.hpp"
#include "DataFlow.hpp"

namespace verilog
{

void CodeBlock::dump()
{
	std::cout << "//" << name << ":\n";
	for (DataFlow *dataflow : dataflows)
	{
		std::cout << "\t";
		dataflow->dump();
	}
}

}

