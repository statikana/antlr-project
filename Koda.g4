grammar Koda;
import KodaTokens;

program		: statement* EOF;

statement	: assignment ENDLINE | expression ENDLINE;

block		: '{' (block | statement)* '}';
assignment	: ID op = operator? ASSIGNMENT expression;

// anything that returns a value
expression
	: <assoc = right> lhs = expression op = POW rhs = expression		# ArithmeticExpr
	| lhs = expression op = (MUL | DIV | FLD | MOD) rhs = expression	# ArithmeticExpr
	| lhs = expression op = (ADD | SUB) rhs = expression				# ArithmeticExpr
	| lhs = expression op = bw_operator rhs = expression				# BitwiseExpr
	| lhs = expression op = comparer rhs = expression					# ComparisonExpr
	| atom																# AtomExpr;

atom
	: '(' expression ')'					# ParenthesesAtom
	| BOOLEAN								# BooleanAtom
	| NUMBER								# NumberAtom
	| ID									# IDAtom
	| STRING								# StringAtom
	| ARRAY_ENTER array_inside ARRAY_CLOSE	# ArrayAtom
	;

array_inside: (expression ARRAY_SEP)* expression?;

comparer	: GT | LT | GE | LE | EQ | NOT_EQ | IS | IS_NOT;

operator	: arith_operator | bw_operator;

arith_operator
	: ADD | SUB
	| MUL | DIV | MOD | FLD | POW
	;

bw_operator
	: BW_AND
	| BW_OR
	| BW_XOR
	| BW_NOT
	| BW_LSHIFT
	| BW_RSHIFT;