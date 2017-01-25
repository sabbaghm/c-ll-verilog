/**
 * @file   Argument.cc
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  This files defines the methods for the verilog statement arguments class.
 */

#include <iostream>

#include "Argument.h"
#include "CodeBlock.h"


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

