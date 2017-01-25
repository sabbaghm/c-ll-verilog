/**
 * @file   CodeBlock.cc
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  This files dumps the code block name and its contents in a proper manner.
 */

#include "CodeBlock.h"
#include "DataFlow.h"

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

