grammar Koda;
import KodaTokens;

// program: series of statements
program		: statement* EOF;

// statement: assignment, expression, or definition
statement	: (assignment | expression | definition) ENDLINE;

// block: enter a new scope
block		: '{' (block | statement)* '}';

// assignment: id, operator, expression
// assignment	: ID op = operator? ASSIGNMENT expression;
assignment	: ID ASSIGNMENT expression;

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
	: func_name=ID '(' params=funcDefParamsList ')' block
	# FunctionDefinition
	;

funcDefParamsList:
	(ID ID ',')* (ID ID ','?)?;