#ifndef VERILOG_ARGUMENT_H
#define VERILOG_ARGUMENT_H

#include <string>
#include <sstream>
#include <iomanip>

namespace verilog
{

// Forward declarations
class CodeBlock;


class Argument
{
public:
	// Dump the argument (abstract function)
	virtual void dump() = 0;
	virtual int getWidth() = 0;
	virtual void setWidth(int val) = 0;
};

class Wire : public Argument
{
public:

	// Data-flow direction
	enum Direction
	{
		DirectionInvalid,
		DirectionLhs,
		DirectionRhs,
		DirectionInput,
		DirectionOutput
	};
//The make_string function is taken from http://stackoverflow.com/questions/8713643/how-to-generate-consecutive-c-strings
	static std::string make_string(const std::string& a_prefix,
                        size_t a_suffix,
                        size_t a_max_length)
	{
		std::ostringstream result;
		result << a_prefix <<
		std::setfill('0') <<
		std::setw(a_max_length - a_prefix.length()) <<
		a_suffix;
		return result.str();
	}

private:

	// Internal, Input or output wire
	Direction direction = DirectionInvalid;

	// Wire name
	std::string name;

	// Wire width
	int width;

	// Wire ID counter
	static int next_number;
	
//	Make_string make_string;


public:

	// Return a new wire argument.
	static Wire *newWire(Direction direction)
	{
		return new Wire(make_string("wire", next_number++, 7), direction);
	}

	// Constructor
	Wire(std::string name, Direction direction) :
			name(name),
			direction(direction)
	{
	}

	// Wire internal, input or output direction
	Direction getDirection() { return direction; }
	//Direction getDirection();
	
	// Return wire name
	std::string getName() { return name; }

	//Return the wire width
	int getWidth();
	
	//Set the wire width
	void setWidth(int val);

	// Print argument
	void dump();

//	void test();
};

class Register : public Argument
{
public:

	// Data-flow direction
	enum Direction
	{
		DirectionInvalid,
		DirectionLhs,
		DirectionRhs,
		DirectionInput,
		DirectionOutput
	};
	
//The make_string function is taken from http://stackoverflow.com/questions/8713643/how-to-generate-consecutive-c-strings
	static std::string make_string(const std::string& a_prefix,
                        size_t a_suffix,
                        size_t a_max_length)
	{
		std::ostringstream result;
		result << a_prefix <<
		std::setfill('0') <<
		std::setw(a_max_length - a_prefix.length()) <<
		a_suffix;
		return result.str();
	}

private:

	// Internal, Input or output register
	Direction direction = DirectionInvalid;

	// Register name
	std::string name;

	// Register width
	int width;

	// True if register is used in sequential logic (flip-flop) false if it is a wire
	bool seqReg = false;

	// Register ID counter
	static int next_number;

//	Make_string make_string;

public:

	// Return whether a register is actually a wire. This is
	// used for instruction printing purposes.
	static bool isSeqRegister(Argument *argument)
	{
		Register *r = dynamic_cast<Register *>(argument);
		return r && r->isSequential();
	}

	// Return a new register argument.
	static Register *newRegister(Direction direction, bool seqReg = false)
	{
		return new Register(make_string("reg", next_number++, 7), direction, seqReg);
	}

	// Constructor
	Register(std::string name, Direction direction, bool seqReg = false) :
			name(name),
			direction(direction),
			seqReg(seqReg)
	{
	}

	// Return internal, input or output direction
	Direction getDirection() { return direction; }
	//Direction getDirection();

	// Return register number
	std::string getName() { return name; }
	
	//Return the register width
	int getWidth();
	
	//Set the register width
	void setWidth(int val);

	// Return whether register is a sequential register (flip-flop)
	bool isSequential() { return seqReg; }

	// Print argument
	void dump();
	//void test();
};


class Constant : public Argument
{
	// Value of the constant
	int value;

	int width;
public:

	// Constructor
	Constant(int value) : value(value)
	{
	}

	// Return constant value
	int getValue() { return value; }
	
	//Return the register width
	int getWidth();
	
	//Set the register width
	void setWidth(int val);

	// Print argument
	void dump();
//	void test();
};


}
#endif
