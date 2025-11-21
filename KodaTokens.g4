lexer grammar KodaTokens;

ML_COMMENT      : ('/.' (. | NEWLINE)*? './') -> skip;

ONELINE_COMMENT: '//' ~[\r\n]* -> skip;

REPEAT_NEWLINE  : [\r\n]+ REPEAT_NEWLINE -> skip;


fragment ALPHA  : [a-zA-Z_];
fragment DIGIT  : [0-9];

BOOLEAN         : 'true' | 'false';
VOID            : 'void';
NUMBER          : ('-'? DIGIT+ ('.' DIGIT*)?) | ('-'? '.' DIGIT+);
HEX             : HEX_PREFIX (DIGIT | [a-fA-F])+;
BIN             : BINARY_PREFIX [01]+;

fragment HEX_PREFIX      : '0x';
fragment BINARY_PREFIX   : '0b';


ID              : ALPHA (DIGIT | ALPHA)*;


fragment STRING_CONTENT: (.)*?;
STRING: '\'' STRING_CONTENT '\'' | '"' STRING_CONTENT '"';


fragment ITEM: BOOLEAN | VOID | NUMBER | HEX | BIN | ID | STRING;

fragment COMMA_SEP_ITEMS: (ITEM ',')* (ITEM ','?)?;


ARRAY: '[' COMMA_SEP_ITEMS ']';

// comparisons
// COMP_GT      : '>';
// COMP_LT      : '<';
// COMP_GE      : '>=';
// COMP_LE      : '<=';
// COMP_EQ      : '==';
// COMP_NEQ     : '!=';
// IS      : '===';
// ISNT    : '!==';


// arithmetic
ARITH_ADD     : '+';
ARITH_SUB     : '-';
ARITH_MUL     : '*';
ARITH_DIV     : '/';
ARITH_MOD     : '%';
ARITH_FLD     : '\\';
ARITH_POW     : '**';


// // bitwise
// BW_OR       : '|';
// BW_AND      : '&';
// BW_XOR      : '^';
// BW_LSH      : '<<';
// BW_RSH      : '>>';

// singleton operators
// LOGIC_NOT   : '!';
// BW_NOT      : '~';


ASSIGNMENT  : '=';


// array rules
// ARRAY_ENTER : '[';
// ARRAY_CLOSE : ']';
// ARRAY_SEP   : ',';

// these rules only catch things not caught by comments etc.
NEWLINE : '\r'? '\n' -> skip;
WS      : [ \t] -> skip;

ENDLINE : ';';