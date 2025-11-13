grammar main;

program: statement* EOF ;

statement
    : assignment ENDLINE
    | expression ENDLINE
    ;

block       : '{' (block | statement)* '}';
assignment  : ID assigner expression;


// anything that returns a value
expression
    : <assoc=right> lhs=expression op=POW                       rhs=expression  # ArithmeticExpr
    |               lhs=expression op=(MUL | DIV | FLD | MOD)   rhs=expression  # ArithmeticExpr
    |               lhs=expression op=(ADD | SUB)               rhs=expression  # ArithmeticExpr
    
    |               lhs=expression comparer                     rhs=expression  # ComparisonExpr
    
    | atom                                                                      # AtomExpr
    ;


atom
    : '(' expression ')'    # ParenthesesAtom
    | NUMBER                # NumberAtom
    | ID                    # IDAtom    
    | STRING                # StringAtom
    ;

comparer    : GT | LT | GE | LE | EQ | NOT_EQ | IS | IS_NOT ;
assigner    : ASSIGN_EQ
            | ASSIGN_ADD
            | ASSIGN_SUB
            | ASSIGN_MUL
            | ASSIGN_DIV
            | ASSIGN_MOD
            | ASSIGN_FLD
            | ASSIGN_POW;

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


// assignments
ASSIGN_EQ       : '=';
ASSIGN_ADD      : '+=';
ASSIGN_SUB      : '-=';
ASSIGN_MUL      : '*=';
ASSIGN_DIV      : '/=';
ASSIGN_MOD      : '%=';
ASSIGN_FLD      : '//=';
ASSIGN_POW      : '**=';

// these rules only catch things not caught by comments etc.
NEWLINE
    : '\r'? '\n' 
    -> skip;
WS
    : [ \t]
    -> skip;

ENDLINE: ';';