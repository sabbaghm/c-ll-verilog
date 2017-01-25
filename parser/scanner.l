/**
 * @file   scanner.l
 * @Author Rafael Ubal (ubal@ece.neu.edu), Northeastern University
 * @Author Majid Sabbagh (sabbagh.m@husky.neu.edu), Northeastern University
 * @date   Dec, 2016
 * @brief  The scanner description for FLEX.
 */

%{
#include <iostream>
#include <list>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include "Type.h"
#include "SymbolTable.h"
#include "parser.h"

#define YY_DECL extern "C" int yylex()
	using namespace std;
%}
%option noyywrap
%%

"bool" {
	return TokenBool;
}

"short" {
	return TokenShort;
}

"int" {
	return TokenInt;
}

"float" {
	return TokenFloat;
}

"void" {
	return TokenVoid;
}

"struct" {
	return TokenStruct;
}

"if" {
	return TokenIf;
}

"else" {
	return TokenElse;
}

"while" {
	return TokenWhile;
}

"return" {
	return TokenReturn;
}

"||" {
	return TokenLogicalOr;
}

"&&" {
	return TokenLogicalAnd;
}

">=" {
	return TokenGreaterEqual;
}

"<=" {
	return TokenLessEqual;
}

"==" {
	return TokenDoubleEqual;
}

"!=" {
	return TokenNotEqual;
}

">>" {
	return TokenLogicalShiftRight;
}

"^" {
	return TokenBitwiseXor;
}

"&" {
	return TokenBitwiseAnd;
}

"|" {
	return TokenBitwiseOr;
}

"<" {
	return TokenLessThan;
}

">" {
	return TokenGreaterThan;
}

"!" {
	return TokenLogicalNot;
}

"*" {
	return TokenMult;
}

"/" {
	return TokenDiv;
}

"+" {
	return TokenPlus;
}

"-" {
	return TokenMinus;
}

"." {
	return TokenPoint;
}

"[" {
	return TokenOpenSquare;
}

"]" {
	return TokenCloseSquare;
}

"{" {
	return TokenOpenCurly;
}

"}" {
	return TokenCloseCurly;
}

"(" {
	return TokenOpenPar;
}

")" {
	return TokenClosePar;
}

";" {
	return TokenSemicolon;
}

"," {
	return TokenComma;
}

"=" {
	return TokenEqual;
}

[a-zA-Z_][a-zA-Z_0-9]* {
	yylval.name = strdup(yytext);
	return TokenId;
}

[0-9]+ {
	yylval.value = atoi(yytext);
	return TokenNumber;
}

[ \t\n]+ {
	// Skip spaces, tabs, and newlines
}

. {
	std::cout << "Invalid token\n";
}
%%
