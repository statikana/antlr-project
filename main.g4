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
    : expression POWER expression                           # PowerExpr
    | expression (MUL | DIV | FLOOR_DIV | MOD) expression   # MulDivExpr
    | expression (ADD | SUB) expression                     # AddSubExpr
    
    | expression comparer expression                        # ComparisonExpr
    
    | atom                                                  # AtomExpr
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
            | ASSIGN_FLOOR_DIV
            | ASSIGN_POWER;

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
NUMBER           : '-'? DIGIT+ ('.' DIGIT+)?;
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
FLOOR_DIV   : '//' ;
POWER       : '**' ;


// assignments
ASSIGN_EQ       : '=';
ASSIGN_ADD      : '+=';
ASSIGN_SUB      : '-=';
ASSIGN_MUL      : '*=';
ASSIGN_DIV      : '/=';
ASSIGN_MOD      : '%=';
ASSIGN_FLOOR_DIV: '//=';
ASSIGN_POWER    : '**=';

// these rules only catch things not caught by comments etc.
NEWLINE
    : '\r'? '\n' 
    -> skip;
WS
    : [ \t]
    -> skip;

ENDLINE: ';';