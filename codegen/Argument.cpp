/**
 * @file   Argument.cpp
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  This files defines the methods for the verilog statement arguments class.
 */

/**
 * Update: Support New Pass Manager (tested on LLVM 17/18) and migrate to CPP
 * @author Mohamed Bouaziz (mohamed.bouaziz@kaust.edu.sa), KAUST
 * @date   Oct, 2023
*/

#include <iostream>

#include "Argument.hpp"
#include "CodeBlock.hpp"


namespace verilog
{

int Wire::next_number = 0;
int Register::next_number = 0;

void Wire::dump()
{
	std::cout << name;
}

void Register::dump()
{
	std::cout << name;
}


void Constant::dump()
{
	std::cout << value;
}

int Wire::getWidth()
{
	return width;
}

int Register::getWidth()
{
	return width;
}


int Constant::getWidth()
{
	return width;
}


void Wire::setWidth(int val)
{
	width = val;
}

void Register::setWidth(int val)
{
	width = val;
}

void Constant::setWidth(int val)
{
	width = val;
}

}

