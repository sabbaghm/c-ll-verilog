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

