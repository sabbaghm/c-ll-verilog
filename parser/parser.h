/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TokenBool = 258,
     TokenShort = 259,
     TokenInt = 260,
     TokenFloat = 261,
     TokenVoid = 262,
     TokenStruct = 263,
     TokenId = 264,
     TokenNumber = 265,
     TokenOpenCurly = 266,
     TokenCloseCurly = 267,
     TokenOpenSquare = 268,
     TokenCloseSquare = 269,
     TokenOpenPar = 270,
     TokenClosePar = 271,
     TokenSemicolon = 272,
     TokenComma = 273,
     TokenPoint = 274,
     TokenEqual = 275,
     TokenLogicalOr = 276,
     TokenLogicalAnd = 277,
     TokenLogicalNot = 278,
     Toke = 279,
     TokenGreaterThan = 280,
     TokenLessThan = 281,
     TokenGreaterEqual = 282,
     TokenLessEqual = 283,
     TokenNotEqual = 284,
     TokenDoubleEqual = 285,
     TokenBitwiseOr = 286,
     TokenBitwiseXor = 287,
     TokenBitwiseAnd = 288,
     TokenLogicalShiftRight = 289,
     TokenMinus = 290,
     TokenPlus = 291,
     TokenDiv = 292,
     TokenMult = 293,
     TokenIf = 294,
     TokenThen = 295,
     TokenElse = 296,
     TokenWhile = 297,
     TokenReturn = 298
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 78 "parser.y"

	char *name;
	llvm::Value *llvalue;
	int value;
	Type *type;
	std::list<int> *indices;

	// For LValue
	struct {
		Type *type;
		llvm::Value *lladdress;
		std::vector<llvm::Value *> *llindices;
		llvm::Value *llvalue;
	} lvalue;
	
	// For mid-rule actions in logical expressions
	struct {
		llvm::BasicBlock *lhs_basic_block;
		llvm::BasicBlock *rhs_basic_block;
		llvm::BasicBlock *end_basic_block;
	} logical;

	// For mid-rule actions in "if" statement
	struct {
		llvm::BasicBlock *then_basic_block;
		llvm::BasicBlock *else_basic_block;
		llvm::BasicBlock *end_basic_block;
	} if_statement;
	// For mid-rule actions in "while" statement
	struct {
		llvm::BasicBlock *cond_basic_block;
		llvm::BasicBlock *body_basic_block;
		llvm::BasicBlock *end_basic_block;
	} while_statement;

	// For 'FormalArguments' and 'FormalArgumentsComma'
	std::vector<Symbol *> *formal_arguments;

	// For 'FunctionDeclaration'
	Symbol *symbol;

	// For 'ActualArguments' and 'ActualArgummentsComma'
	std::vector<llvm::Value *> *actual_arguments;


/* Line 2058 of yacc.c  */
#line 146 "parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_H_INCLUDED  */
