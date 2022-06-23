%output "parser.c"          // File name of generated parser.
%defines "parser.h"         // Produces a 'parser.h'
%define parse.error verbose // Give proper messages when a syntax error is found.
%define parse.lac full      // Enable LAC to improve syntax error handling.

%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(char const *s);
%}

%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER INLINE RESTRICT
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token BOOL COMPLEX IMAGINARY
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%precedence ELSE

%start translation_unit
%%

//expressao basica 
primary_expression
	: IDENTIFIER 
	| CONSTANT 
	| STRING_LITERAL  
	| '(' expression ')' 
	;

//expressao posfixa
postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	| '(' type_name ')' '{' initializer_list '}'
	| '(' type_name ')' '{' initializer_list ',' '}'
	;

//argumentos de uma expressao
argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

//expressoes unarias
unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;

//operadores unarios
unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

//expressoes de casting
cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression
	;

//expressoes de multiplicacao
multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

//expressao de soma
additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

//expressoes de shift left e right
shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

//expressoes de relacoes
relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

//expressoes de igualdade
equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

//expressoes logicas AND
and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

//expressoes logicas OR exclusivo
exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

//expressoes logicas OR
inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

//expressoes logicas geral AND
logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

//expressoes logicas geral OR
logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

//expressoes condicionais
conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

//expressao para atribuir valor
assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
	;

//oporadores de atribuicao
assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

//expressao basica
expression
	: assignment_expression
	| expression ',' assignment_expression
	;


constant_expression
	: conditional_expression
	;

//declaracoes
declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
	;

//espicificacoes de declaracoes
declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier
	| type_specifier declaration_specifiers
	| type_qualifier
	| type_qualifier declaration_specifiers
	| function_specifier
	| function_specifier declaration_specifiers
	;

//inicio da declaracao
init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

//declarador
init_declarator
	: declarator
	| declarator '=' initializer
	;

//especificacao tipo de armazenamento
storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

//especificacao de tipo da funcao
type_specifier
	: VOID
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| BOOL
	| COMPLEX
	| IMAGINARY
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME
	;

/* declaracoes  
struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;*/

/* struct e union
struct_or_union
	: STRUCT
	| UNION
	;*/

/* lista de structs 
struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;*/

/* declaracao de struct
struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;*/

/* especificacao lista
specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;*/

/* lista de declaracoes
struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;*/

/* declaracao da struct
struct_declarator
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;*/

//especificacao enum
enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER
	;

//lista de enums
enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

//enums
enumerator
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;

//qualificacao de tipo
type_qualifier
	: CONST
	| RESTRICT
	| VOLATILE
	;

//especificacao de funcoes
function_specifier
	: INLINE
	;

//declarador
declarator
	: pointer direct_declarator
	| direct_declarator
	;

// declarador direto
direct_declarator
	: IDENTIFIER
	| '(' declarator ')'
	| direct_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_declarator '[' type_qualifier_list ']'
	| direct_declarator '[' assignment_expression ']'
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list '*' ']'
	| direct_declarator '[' '*' ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
	;

//ponteiro
pointer
	: '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;

//lista de tipo qualificador
type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;

//lista de tipo parametros 
parameter_type_list
	: parameter_list
	| parameter_list ',' ELLIPSIS
	;

//lista de parametros
parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

//declaracao de parametros
parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers
	;

//indentificador da lista
identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

initializer
	: assignment_expression
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| designation initializer
	| initializer_list ',' initializer
	| initializer_list ',' designation initializer
	;

designation
	: designator_list '='
	;

designator_list
	: designator
	| designator_list designator
	;

designator
	: '[' constant_expression ']'
	| '.' IDENTIFIER
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement
	: '{' '}'
	| '{' block_item_list '}'
	;

block_item_list
	: block_item
	| block_item_list block_item
	;

block_item
	: declaration
	| statement
	;

expression_statement
	: ';'
	| expression ';'
	;

selection_statement
	: IF '(' expression ')' statement
	| IF '(' expression ')' statement ELSE statement
	| SWITCH '(' expression ')' statement
	;

iteration_statement
	: WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' statement
	| FOR '(' expression_statement expression_statement expression ')' statement
	| FOR '(' declaration expression_statement ')' statement
	| FOR '(' declaration expression_statement expression ')' statement
	;

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement
	| declaration_specifiers declarator compound_statement
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

%%

#include <stdio.h>

extern char yytext[];
extern int column;

void yyerror(char const *s)
{
	printf("%s\n", s);
}

int main(void) {
    if (yyparse() == 0) printf("PARSE SUCCESSFUL!\n");
    else                printf("PARSE FAILED!\n");
    return 0;
}