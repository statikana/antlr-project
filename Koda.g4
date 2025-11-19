grammar Koda;
import KodaTokens;

// program: series of statements
program		: statement* EOF;

// statement: expression or definition
statement	: (expression | definition) ENDLINE;

// block: enter a new scope
block		: '{' (block | statement)* '}';

// anything that returns a value
expression
	// ARITHMETIC
	: <assoc = right> lhs = expression op = ARITH_POW rhs = expression
	# ArithmeticExpr
	| lhs = expression op = (ARITH_MUL | ARITH_DIV | ARITH_FLD | ARITH_MOD) rhs = expression
	# ArithmeticExpr
	| lhs = expression op = (ARITH_ADD | ARITH_SUB) rhs = expression
	# ArithmeticExpr
	
	// in expression alternatives
	| func_name=ID args=funcCallArgsList
	# FunctionCall

	| atom
	# AtomExpr
	;


funcCallArgsList
	: '(' (ID ID (',' ID ID)*)? ')'
	;

// smallest possible unit of value (core types)
atom
	: '(' expression ')'					# ParenthesesAtom
	| BOOLEAN								# BooleanAtom

	| NUMBER								# NumberAtom
	| HEX									# HexAtom
	| BIN									# BinaryAtom

	| ID									# IDAtom
	| STRING								# StringAtom
	;

operator
	: ARITH_ADD | ARITH_SUB
	| ARITH_MUL | ARITH_DIV
	| ARITH_MOD | ARITH_FLD
	| ARITH_POW
	;

definition
	: 'func ' func_name=ID '(' params=funcDefParamsList ')' '->' ret_t=ID block
	# FunctionDefinition

	| type=ID name=ID ASSIGNMENT expression
	# VariableDefinition
	;

funcDefParamsList:
	(ID ID ',')* (ID ID ','?)?;