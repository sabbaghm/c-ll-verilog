#ifndef TYPE_H
#define TYPE_H

#include <llvm/IR/Type.h>
#include <vector>

// Forward declarations
class SymbolTable;
class Symbol;


class Type
{
public:

	// List of possible type kinds
	enum Kind
	{
		KindInvalid,
		KindConstInt,
		KindConstFloat,
		KindBool,
		KindShort,
		KindInt,
		KindFloat,
		KindVoid,
		KindArray,
		KindStruct,
		KindPointer,
		KindFunction
	};

private:
	// Type kind
	Kind kind;

public:

	/// Constructor
	Type(Kind kind) : kind(kind) { }

	/// Return the type kind
	Kind getKind() {return kind; }

	/// For types of kind 'KindArray', number of elements in array.
	int num_elem = 0;

	/// For types of kind 'KindStruct', symbol table containing the fields.
	SymbolTable *symbol_table = nullptr;

	/// Associated LLVM type
	llvm::Type *lltype = nullptr;

	/// For types of kind 'KindArray' and 'KindPointer', type of array
	/// elements or elements pointed to.
	Type *subtype = nullptr;

	/// For types of kind 'KindFunction'
	std::vector<Symbol *> *arguments;
	Type *rettype = nullptr;


	/// Dump information about the type
	void dump(int indent = 0);
};	

#endif
