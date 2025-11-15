grammar Koda;
import KodaTokens;

program		: statement* EOF;

statement	: assignment ENDLINE | expression ENDLINE;

block		: '{' (block | statement)* '}';
assignment	: ID op = operator? ASSIGNMENT expression;

// anything that returns a value
expression
	: <assoc = right> lhs = expression op = ARITH_POW rhs = expression
	# ArithmeticExpr

	| lhs = expression op = (ARITH_MUL | ARITH_DIV | ARITH_FLD | ARITH_MOD) rhs = expression
	# ArithmeticExpr

	| lhs = expression op = (ARITH_ADD | ARITH_SUB) rhs = expression
	# ArithmeticExpr

	| lhs = expression op = bw_operator rhs = expression
	# BitwiseExpr

	| lhs = expression op = comparer rhs = expression
	# ComparisonExpr

	| op=singleton_operator expression
	# SingletonOperatorExpr

	| atom
	# AtomExpr
	;

atom
	: '(' expression ')'					# ParenthesesAtom
	| BOOLEAN								# BooleanAtom
	| NUMBER								# NumberAtom
	| HEX									# HexAtom
	| BIN									# BinaryAtom
	| ID									# IDAtom
	| STRING								# StringAtom
	| ARRAY_ENTER array_inside ARRAY_CLOSE	# ArrayAtom
	;

array_inside: (expression ARRAY_SEP)* expression?;

comparer	: COMP_GT | COMP_LT | COMP_GE | COMP_LE | COMP_EQ | COMP_NEQ; // | IS | ISNT;

operator	: arith_operator | bw_operator;

arith_operator
	: ARITH_ADD | ARITH_SUB
	| ARITH_MUL | ARITH_DIV
	| ARITH_MOD | ARITH_FLD
	| ARITH_POW
	;

bw_operator
	: BW_AND
	| BW_OR
	| BW_XOR
	| BW_LSH
	| BW_RSH
	;

singleton_operator
	: LOGIC_NOT
	| BW_NOT
	;