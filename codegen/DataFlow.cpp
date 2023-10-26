/**
 * @file   DataFlow.cpp
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  Dataflow class definition.
 */

/**
 * Update: Support New Pass Manager (tested on LLVM 17/18) and migrate to CPP
 * @author Mohamed Bouaziz (mohamed.bouaziz@kaust.edu.sa), KAUST
 * @date   Oct, 2023
*/

#include "llvm/IR/Function.h"
#include "DataFlow.hpp"


namespace verilog
{

std::string DataFlow::names[DataFlow::OpcodeMax] = {
	"",
	"portliststart",
	"iportdefine",
	"oportdefine",
	"portlistend",
	"signaldefine",
	"assign",
	"+",
	"-",
	">>",
	"^",
	"&",
	"|",
	"==",
	">",
	"endmodule"
};


void DataFlow::dump()
{
	int iter = 0;

	// operation name
	std::string op = names[opcode];
	std::string next_string = ";";
	std::string comma = ", ";
	std::string assignment = " =";
	
	if(op == "portliststart"){
		std::cout << "module "; 
		for (Argument *argument : arguments)
		//for (std::list<Argument*>::iterator argument=arguments.begin(); argument != arguments.end(); ++argument)
		{				
			argument->dump();
		}
			

		std::cout << " (";

	} else if(op == "iportdefine"){
		
		//std::cout << "\b\b\b\b\b\b";
		for (Argument *argument : arguments)
		//for (std::list<Argument*>::iterator argument=arguments.begin(); argument != arguments.end(); ++argument)
		{
				
		if (argument->getWidth() == 32)
		{
			std::cout << "input wire [31:0] ";
		}
		else if (argument->getWidth() == 16)
		{
			std::cout << "input wire [15:0] ";
		}
		else if (argument->getWidth() == 8)
		{
			std::cout << "input wire [7:0] ";
		}
		else if (argument->getWidth() == 1)
		{
			std::cout << "input wire ";
		}
		else
		{
			std::cerr << "Unsupported width in module input ports\n";
			exit(1);
		}
			argument->dump();
			std::cout << comma;

		}
			
	} else if(op == "oportdefine"){
		
		//std::cout << "\b\b\b\b\b\b";
		for (Argument *argument : arguments)
		//for (std::list<Argument*>::iterator argument=arguments.begin(); argument != arguments.end(); ++argument)
		{
				
		if (argument->getWidth() == 32)
		{
			std::cout << "output wire [31:0] ";
		}
		else if (argument->getWidth() == 16)
		{
			std::cout << "output wire [15:0] ";
		}
		else if (argument->getWidth() == 8)
		{
			std::cout << "output wire [7:0] ";
		}
		else if (argument->getWidth() == 1)
		{
			std::cout << "output wire ";
		}
		else
		{
			std::cerr << "Unsupported width in module output port\n";
			exit(1);
		}
			argument->dump();

		}
			
	} else if(op == "portlistend"){
		
		//std::cout << "\b\b\b\b";
		std::cout << ");";
/*		for (Argument *argument : arguments)
		//for (std::list<Argument*>::iterator argument=arguments.begin(); argument != arguments.end(); ++argument)
		{			
			argument->dump();
		}
			
*/


	} else if(op == "signaldefine"){
		
		for (Argument *argument : arguments)
		//for (std::list<Argument*>::iterator argument=arguments.begin(); argument != arguments.end(); ++argument)
		{
		if (argument->getWidth() == 32)
		{
			std::cout << "wire [31:0] ";
		}
		else if (argument->getWidth() == 16)
		{
			std::cout << "wire [15:0] ";
		}
		else if (argument->getWidth() == 8)
		{
			std::cout << "wire [7:0] ";
		}
		else if (argument->getWidth() == 1)
		{
			std::cout << "wire ";
		}
		else
		{
			std::cerr << "Unsupported width for internal signals\n";
			exit(1);
		}
				
			argument->dump();

		}
		std::cout << ";";
	
			
	} else if(op != "endmodule"){
		// assign
		std::cout << "assign ";
		for (Argument *argument : arguments)
		//for (std::list<Argument*>::iterator argument=arguments.begin(); argument != arguments.end(); ++argument)
		{
				
			argument->dump();

			if(iter == 0){
				std::cout << assignment;
			} else {	
				next_string = ((op == "assign") | (iter == (arguments.size() - 1) )) ? ";": " " + op;
				std::cout << next_string;				
			}
//			std::cout << op << "\n";				
			// Print argument
			
			// Print space
			std::cout << " ";


		iter++;

		}
	
	} else {
		std::cout << op;
	}
	
	std::cout << "\n";

}


}
