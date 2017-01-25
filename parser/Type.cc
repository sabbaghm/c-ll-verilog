#include <iostream>

#include "Type.h"
#include "SymbolTable.h"

void Type::dump(int indent)
{
	// Indent
	std::cout << std::string(indent, ' ');

	// Kind
	std::cout << "Type: kind=";
	switch (kind)
	{
		case KindConstInt: std::cout << "ConstInt"; break;
		case KindConstFloat: std::cout << "ConstFloat"; break;
		case KindBool: std::cout << "Bool"; break;
		case KindShort: std::cout << "Short"; break;
		case KindInt: std::cout << "Int"; break;
		case KindFloat: std::cout << "Float"; break;
		case KindVoid: std::cout << "Void"; break;
		case KindArray: std::cout << "Array"; break;
		case KindStruct: std::cout << "Struct"; break;
		case KindPointer: std::cout << "Pointer"; break;
		case KindFunction: std::cout << "Function"; break;
	}

	// Number of elements for arrays
	if (kind == KindArray)
		std::cout << ", num_elem=" << num_elem;
	
	// Newline
	std::cout << '\n';

	// Subtypes
	if (subtype)
		subtype->dump(indent + 2);

	// Symbol table
	if (symbol_table)
		symbol_table->dump(indent + 2);
}
