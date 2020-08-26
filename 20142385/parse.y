%{

#include "type.h"
extern int line_no, syntax_err;
extern A_NODE *root;
extern A_ID *current_id;
extern int current_level;
extern A_TYPE *int_type;

%}

%start program
%token IDENTIFIER TYPE_IDENTIFIER
FLOAT_CONSTANT INTEGER_CONSTANT
CHARACTER_CONSTANT STRING_LITERAL PLUS MINUS AMP BARBAR
AMPAMP SEMICOLON LSS GTR LEQ GEQ EQL NEQ LP RP LB RB LR RR
PERIOD COMMA EXCL STAR SLASH PERCENT ASSIGN COLON
AUTO_SYM STATIC_SYM ENUM_SYM IF_SYM ELSE_SYM WHILE_SYM
CONTINUE_SYM BREAK_SYM RETURN_SYM
%%

program
	: translation_unit {root=makeNode(N_PROGRAM,NIL,$1,NIL);}
	;

translation_unit
	: external_declaration {$$=$1;}
	| translation_unit external_declaration {$$=linkDeclaratorList($1,$2);}
	;

external_declaration
	: function_definition {$$=$1;}
	| declaration {$$=$1;}
	;

function_definition
	: declaration_specifiers declarator {$$=setFunctionDeclaratorSpecifier($2,$1);}
	compound_statement{$$=setFunctionDeclaratorBody($3,$4);current_id=$2;}
	;

declaration_list_opt
	: {$$=NIL;}
	| declaration_list {$$=$1;}
	;

declaration_list
	: declaration {$$=$1;}
	| declaration_list declaration {$$=linkDeclaratorList($1,$2);}
	;

declaration
	: declaration_specifiers init_declarator_list_opt SEMICOLON
	{$$=setDeclaratorListSpecifier($2,$1);}
	;

declaration_specifiers
	: type_specifier {$$=makeSpecifier($1,0);}
	| storage_class_specifier {$$=makeSpecifier(0,$1);}
	| type_specifier declaration_specifiers {$$=updateSpecifier($2,$1,0);}
	| storage_class_specifier declaration_specifiers
	{$$=updateSpecifier($2,0,$1);}
	;

storage_class_specifier
	: AUTO_SYM {$$=S_AUTO;}
	| STATIC_SYM {$$=S_STATIC;}
	;

init_declarator_list_opt
	: {$$=makeDummyIdentifier();}
	| init_declarator_list {$$=$1;}
	;

init_declarator_list
	: init_declarator {$$=$1;}
	| init_declarator_list COMMA init_declarator
	{$$=linkDeclaratorList($1,$3);}
	;

init_declarator
	: declarator {$$=$1;}
	;

type_specifier
	: enum_type_specifier {$$ = $1;}
	| TYPE_IDENTIFIER {$$ = $1;}
	;

enum_type_specifier
	: ENUM_SYM IDENTIFIER
	{$$=setTypeStructOrEnumIdentifier(T_ENUM,$2,ID_ENUM);}
	LR enumerator_list RR {$$=setTypeField($3,$5);}
	| ENUM_SYM {$$=makeType(T_ENUM);}
	LR enumerator_list RR {$$=setTypeField($2,$4);}
	| ENUM_SYM IDENTIFIER
	{$$=getTypeOfStructOrEnumRefIdentifier(T_ENUM,$2,ID_ENUM);}
	;

enumerator_list
	: enumerator {$$=$1;}
	| enumerator_list COMMA enumerator {$$=linkDeclaratorList($1,$3);}
	;

enumerator
	: IDENTIFIER
	{$$=setDeclaratorKind(makeIdentifier($1),ID_ENUM_LITERAL);}
	| IDENTIFIER
	{$$=setDeclaratorKind(makeIdentifier($1),ID_ENUM_LITERAL);}
	ASSIGN expression {$$=setDeclaratorInit($2,$4);}
	;

declarator
	: pointer direct_declarator {$$=setDeclaratorElementType($2,$1);}
	| direct_declarator {$$=$1;}
	;

pointer
	: STAR {$$=makeType(T_POINTER);}
	| STAR pointer {$$=setTypeElementType($2,makeType(T_POINTER));}
	;

direct_declarator
	: IDENTIFIER {$$=makeIdentifier($1);}
	| LP declarator RP {$$=$2;}
	| direct_declarator LB constant_expression_opt RB
	{$$=setDeclaratorElementType($1,setTypeExpr(makeType(T_ARRAY),$3));}
	| direct_declarator LP  RP
	{$$=setDeclaratorElementType($1,setTypeField(makeType(T_FUNC),NIL));}
	;

statement_list_opt
	: {$$=makeNode(N_STMT_LIST_NIL,NIL,NIL,NIL);}
	| statement_list {$$=$1;}
	;

statement_list
	: statement {$$=makeNode(N_STMT_LIST,$1,NIL, makeNode(N_STMT_LIST_NIL,NIL,NIL,NIL));}
	| statement_list statement {$$=makeNodeList(N_STMT_LIST,$1,$2);}
	;

statement
	: compound_statement {$$=$1;}
	| expression_statement {$$=$1;}
	| selection_statement {$$=$1;}
	| iteration_statement {$$=$1;}
	| jump_statement {$$=$1;}
	;

compound_statement
	: LR {$$=current_id;current_level++;} declaration_list_opt statement_list_opt RR 
	{$$=makeNode(N_STMT_COMPOUND,$3,NIL,$4); current_id=$2;	current_level--;}
	;

expression_statement
	: SEMICOLON {$$=makeNode(N_STMT_EMPTY,NIL,NIL,NIL);}
	| expression SEMICOLON {$$=makeNode(N_STMT_EXPRESSION,NIL,$1,NIL);}
	;

selection_statement
	: IF_SYM LP expression RP statement
	{$$=makeNode(N_STMT_IF,$3,NIL,$5);}
	| IF_SYM LP expression RP statement ELSE_SYM statement
	{$$=makeNode(N_STMT_IF_ELSE,$3,$5,$7);}
	;

iteration_statement
	: WHILE_SYM LP expression RP statement
	{$$=makeNode(N_STMT_WHILE,$3,NIL,$5);}
	;

expression_opt
	: /* empty */ {$$=NIL;}
	| expression {$$=$1;}
	;

jump_statement
	: RETURN_SYM expression_opt SEMICOLON
	{$$=makeNode(N_STMT_RETURN,NIL,$2,NIL);}
	| CONTINUE_SYM SEMICOLON
	{$$=makeNode(N_STMT_CONTINUE,NIL,NIL,NIL);}
	| BREAK_SYM SEMICOLON
	{$$=makeNode(N_STMT_BREAK,NIL,NIL,NIL);}
	;

constant_expression_opt
	: {$$=NIL;}
	| constant_expression {$$=$1;}
	;

constant_expression
	: expression {$$=$1;}
	;

expression
	: logical_or_expression {$$=$1;}
	| unary_expression ASSIGN expression
	{$$=makeNode(N_EXP_ASSIGN,$1,NIL,$3);}
	;

logical_or_expression
	: logical_and_expression {$$=$1;}
	| logical_or_expression BARBAR logical_and_expression
	{$$=makeNode(N_EXP_OR,$1,NIL,$3);}
	;

logical_and_expression
	: equality_expression
	{$$=$1;}
	| logical_and_expression AMPAMP equality_expression
	{$$=makeNode(N_EXP_AND,$1,NIL,$3);}
	;

equality_expression
	: relational_expression {$$=$1;}
	| equality_expression EQL relational_expression
	{$$=makeNode(N_EXP_EQL,$1,NIL,$3);}
	| equality_expression NEQ relational_expression
	{$$=makeNode(N_EXP_NEQ,$1,NIL,$3);}
	;

relational_expression
	: additive_expression {$$=$1;}
	| relational_expression LSS additive_expression
	{$$=makeNode(N_EXP_LSS,$1,NIL,$3);}
	| relational_expression GTR additive_expression
	{$$=makeNode(N_EXP_GTR,$1,NIL,$3);}
	| relational_expression LEQ additive_expression
	{$$=makeNode(N_EXP_LEQ,$1,NIL,$3);}
	| relational_expression GEQ additive_expression
	{$$=makeNode(N_EXP_GEQ,$1,NIL,$3);}
	;

additive_expression
	: multiplicative_expression {$$=$1;}
	| additive_expression PLUS multiplicative_expression
	{$$=makeNode(N_EXP_ADD,$1,NIL,$3);}
	| additive_expression MINUS multiplicative_expression
	{$$=makeNode(N_EXP_SUB,$1,NIL,$3);}
	;

multiplicative_expression
	: unary_expression {$$=$1;}
	| multiplicative_expression STAR unary_expression
	{$$=makeNode(N_EXP_MUL,$1,NIL,$3);}
	| multiplicative_expression SLASH unary_expression
	{$$= makeNode(N_EXP_DIV,$1,NIL,$3);}
	| multiplicative_expression PERCENT unary_expression
	{$$= makeNode(N_EXP_MOD,$1,NIL,$3);}
	;

unary_expression
	: postfix_expression {$$=$1;}
	| AMP unary_expression {$$=makeNode(N_EXP_AMP,NIL,$2,NIL);}
	| STAR unary_expression {$$=makeNode(N_EXP_STAR,NIL,$2,NIL);}
	| EXCL unary_expression {$$=makeNode(N_EXP_NOT,NIL,$2,NIL);}
	| MINUS unary_expression {$$=makeNode(N_EXP_MINUS,NIL,$2,NIL);}
	| PLUS unary_expression {$$=makeNode(N_EXP_PLUS,NIL,$2,NIL);}
	;

postfix_expression
	: primary_expression {$$=$1;}
	| postfix_expression LB expression RB
	{$$=makeNode(N_EXP_ARRAY,$1,NIL,$3);}
	| postfix_expression LP RP
	{$$=makeNode(N_EXP_FUNCTION_CALL,$1,NIL,NIL);}
	;

primary_expression
	: IDENTIFIER
	{$$=makeNode(N_EXP_IDENT,NIL,getIdentifierDeclared($1),NIL);}
	| INTEGER_CONSTANT {$$=makeNode(N_EXP_INT_CONST,NIL,$1,NIL);}
	| FLOAT_CONSTANT {$$=makeNode(N_EXP_FLOAT_CONST,NIL,$1,NIL);}
	| CHARACTER_CONSTANT{$$=makeNode(N_EXP_CHAR_CONST,NIL,$1,NIL);}
	| STRING_LITERAL {$$=makeNode(N_EXP_STRING_LITERAL,NIL,$1,NIL);}
	| LP expression RP {$$=$2;}
	;

%%

extern char *yytext;

yyerror(char *s)
{
	syntax_err++;
	printf("line %d: %s near %s \n", line_no, s,yytext);
}
