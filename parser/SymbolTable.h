#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <string>
#include <llvm/IR/Module.h>

// Forward declaration
class Type;

class Symbol
{
	// Symbol name
	std::string name;

	// Index of next temporary variable
	static int temp_index;

	// Index of next basic block
	static int basic_block_index;

public:
	
	/// Constructor
	Symbol(const std::string &name) : name(name) { }

	/// Return the symbol name
	const std::string &getName() { return name; }

	/// Symbol type
	Type *type = nullptr;

	/// Index for a symbol in a symbol table with scope set to
	/// 'ScopeStruct'
	int index = 0;

	/// LLVM address of the variable for symbols in a symbol table with
	/// its scope set to 'ScopeGlobal' or 'ScopeLocal'.
	llvm::Value *lladdress = nullptr;

	/// Dump information about the symbol
	void dump(int indent = 0);

	/// Return the name of a new temporary variable
	static std::string getTemp();

	/// Return the name of a new basic block
	static std::string getBasicBlock();
};

class SymbolTable
{
public:

	/// Possible scopes for the symbol table
	enum Scope
	{
		ScopeInvalid,
		ScopeGlobal,
		ScopeLocal,
		ScopeStruct
	};

private:

	// Symbol table scope
	Scope scope;
	
	// Symbols
	std::unordered_map<std::string, Symbol *> symbols;

public:

	/// Constructor
	SymbolTable(Scope scope) : scope( scope ) { }

	/// Return the symbol table scope
	Scope getScope() { return scope; }

	/// Return a symbol given its name, or nullptr if not found.
	Symbol *getSymbol(const std::string &name);

	/// Add symbol to table
	void addSymbol(Symbol *symbol)
	{
		symbols[symbol->getName()] = symbol;
	}

	/// Dump symbol table to standard output
	void dump(int indent = 0);

	/// Return number of symbols in the symbol table. This is useful to
	/// calculate the index of the next symbol in a data structure
	int size() { return symbols.size(); }

	/// Return the LLVM types of all symbols in the symbol table, sorted
	/// by their index. This is useful for constructing LLVM struct types.
	void getLLVMTypes(std::vector<llvm::Type *> &types);

	};

#endif
