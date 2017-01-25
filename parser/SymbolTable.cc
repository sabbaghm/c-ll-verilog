/**
 * @file   SymbolTable.h
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  The symbol table class definition for LLVM parser.
 */

#include <iostream>
#include <sstream>

#include "SymbolTable.h"
#include "Type.h"

void Symbol::dump(int indent)
{
	// Indent
	std::cout << std::string(indent, ' ');

	// Name, index
	std::cout << "Symbol: name='" << name << "', index=" << index << '\n';

	// Type
	type->dump(indent + 2);
}

int Symbol::temp_index = 0;
int Symbol::basic_block_index = 0;

std::string Symbol::getTemp()
{
	std::ostringstream stream;
	stream << "t" << temp_index++;
	return stream.str();
}

std::string Symbol::getBasicBlock()
{
	std::ostringstream stream;
	stream << "L" << basic_block_index++;
	return stream.str();
}


Symbol *SymbolTable::getSymbol(const std::string &name)
{
	// Search
	auto it = symbols.find(name);

	// Not found
	if (it == symbols.end())
		return nullptr;

	// Return symbol
	return it->second;
}

void SymbolTable::dump(int indent)
{
	// Indent
	std::cout << std::string(indent, ' ');

	// Scope
	std::cout << "Symbol table: scope=";
	switch (scope)
	{
		case ScopeGlobal: std::cout << "global"; break;
		case ScopeLocal: std::cout << "local"; break;
		case ScopeStruct: std::cout << "struct"; break;
	}


	// Symbols
	std::cout << ", symbols:\n";
	for(auto it : symbols)
		it.second->dump(indent + 2);

	// Newline
	std::cout << '\n';
}

void SymbolTable::getLLVMTypes(std::vector<llvm::Type *> &types)
{
	// Add null symbols to vector
	for (unsigned i = 0; i < symbols.size(); i++)
		types.push_back(nullptr);

	// Add symbols sorted by their index
	for (auto it : symbols)
		types[it.second->index] = it.second->type->lltype;

}
