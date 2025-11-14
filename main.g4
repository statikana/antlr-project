grammar main;

program: statement* EOF ;

statement
    : assignment ENDLINE
    | expression ENDLINE
    ;

block       : '{' (block | statement)* '}';
assignment  : ID op=operator? ASSIGNMENT expression;


// anything that returns a value
expression
    : <assoc=right> lhs=expression op=POW                       rhs=expression  # ArithmeticExpr
    |               lhs=expression op=(MUL | DIV | FLD | MOD)   rhs=expression  # ArithmeticExpr
    |               lhs=expression op=(ADD | SUB)               rhs=expression  # ArithmeticExpr
    |               lhs=expression op=bw_operator               rhs=expression  # BitwiseExpr

    |               lhs=expression op=comparer                  rhs=expression  # ComparisonExpr
    
    | atom                                                                      # AtomExpr
    ;


atom
    : '(' expression ')'    # ParenthesesAtom
    | BOOLEAN               # BooleanAtom
    | NUMBER                # NumberAtom
    | ID                    # IDAtom    
    | STRING                # StringAtom
    ;

comparer        : GT | LT | GE | LE | EQ | NOT_EQ | IS | IS_NOT ;

operator        : arith_operator | bw_operator;

arith_operator  : ADD | SUB | MUL | DIV | MOD | FLD | POW;
bw_operator     : BW_AND | BW_OR | BW_XOR | BW_NOT | BW_LSHIFT | BW_RSHIFT;

//
// LEXER RULES
//
ML_COMMENT
    : ('/.' (.|NEWLINE)*? './')
    -> skip;

ONELINE_COMMENT
    : '//' ~[\r\n]*
    -> skip;

REPEAT_NEWLINE
    : [\r\n]+ REPEAT_NEWLINE
    -> skip;

fragment ALPHA   : [a-zA-Z_];
fragment DIGIT   : [0-9];
NUMBER           : ('-'? DIGIT+ ('.' DIGIT*)?) | ('-'? '.' DIGIT+);
BOOLEAN          : 'true' | 'false';
ID               : ALPHA (DIGIT|ALPHA)* ;


fragment STRING_CONTENT: (.)*?;

STRING  : '\'' STRING_CONTENT '\''
        | '"'  STRING_CONTENT '"'
        ;

// comparisons
GT      : '>' ;
LT      : '<' ;
GE      : '>=' ;
LE      : '<=' ;
EQ      : '==' ;
NOT_EQ  : '!=' ;
IS      : '===' ;
IS_NOT  : '!==' ;


// operators
ADD         : '+' ;
SUB         : '-' ;
MUL         : '*' ;
DIV         : '/' ;
MOD         : '%' ;
FLD         : '//' ;
POW         : '**' ;

BW_OR       : '|';
BW_AND      : '&';
BW_XOR      : '^';
BW_LSHIFT   : '<<';
BW_RSHIFT   : '>>';
BW_NOT      : '~';


ASSIGNMENT  : '=';

// these rules only catch things not caught by comments etc.
NEWLINE
    : '\r'? '\n' 
    -> skip;
WS
    : [ \t]
    -> skip;

ENDLINE: ';';