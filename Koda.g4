grammar Koda;
import KodaTokens;

// program: series of statements
program		: statement* EOF;

// statement: expression or definition
statement	: (expression | definition | control_seq | block) ENDLINE;

block		: '{' statement* '}';

control_seq
	: 'if' '(' first_e=expression ')' block ('elif' '(' inter_e=expression ')' block)* ('else' last_e=block)?
	#IfElseControlSeq
	;


// anything that returns a value
expression
	// ARITHMETIC
	: <assoc = right> lhs = expression op = ARITH_POW rhs = expression
	# ArithmeticExpr
	| lhs = expression op = (ARITH_MUL | ARITH_DIV | ARITH_FLD | ARITH_MOD) rhs = expression
	# ArithmeticExpr
	| lhs = expression op = (ARITH_ADD | ARITH_SUB) rhs = expression
	# ArithmeticExpr

	| atom
	# AtomExpr
	;


// smallest possible unit of value (core types)
atom
	: '(' expression ')'
	# ParenthesesAtom

	| ID '(' (expression ',')* expression? ')'
	# FuncCallAtom

	| '[' (expression ',')* expression? ']'
	# ArrayAtom

	| BOOLEAN
	# BooleanAtom

	| NUMBER
	# NumberAtom

	| VOID
	# VoidAtom

	| ID
	# IDAtom

	| STRING
	# StringAtom
	;

operator
	: ARITH_ADD | ARITH_SUB
	| ARITH_MUL | ARITH_DIV
	| ARITH_MOD | ARITH_FLD
	| ARITH_POW
	;

definition
	: type=ID name=ID
	# VarDef

	| type=ID name=ID ASSIGNMENT expression
	# VarDefAndAssign

	| name=ID ASSIGNMENT expression
	# VarAssign

	| 'func' name=ID '(' (ID ID ',')* (ID ID)? ')' '->' ret_type=ID block
	# FuncDef
	;
