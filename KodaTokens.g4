lexer grammar KodaTokens;

ML_COMMENT      : ('/.' (. | NEWLINE)*? './') -> skip;

ONELINE_COMMENT: '//' ~[\r\n]* -> skip;

REPEAT_NEWLINE  : [\r\n]+ REPEAT_NEWLINE -> skip;

fragment ALPHA  : [a-zA-Z_];
fragment DIGIT  : [0-9];
NUMBER          : ('-'? DIGIT+ ('.' DIGIT*)?) | ('-'? '.' DIGIT+);
BOOLEAN         : 'true' | 'false';
ID              : ALPHA (DIGIT | ALPHA)*;

fragment STRING_CONTENT: (.)*?;

STRING: '\'' STRING_CONTENT '\'' | '"' STRING_CONTENT '"';

// comparisons
GT      : '>';
LT      : '<';
GE      : '>=';
LE      : '<=';
EQ      : '==';
NOT_EQ  : '!=';
IS      : '===';
IS_NOT  : '!==';

// operators
ADD     : '+';
SUB     : '-';
MUL     : '*';
DIV     : '/';
MOD     : '%';
FLD     : '\\';
POW     : '**';

BW_OR       : '|';
BW_AND      : '&';
BW_XOR      : '^';
BW_LSHIFT   : '<<';
BW_RSHIFT   : '>>';
BW_NOT      : '~';

ASSIGNMENT  : '=';

ARRAY_ENTER : '[';
ARRAY_CLOSE : ']';
ARRAY_SEP   : ',';

// these rules only catch things not caught by comments etc.
NEWLINE : '\r'? '\n' -> skip;
WS      : [ \t] -> skip;

ENDLINE : ';';