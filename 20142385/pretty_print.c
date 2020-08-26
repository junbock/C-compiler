#include "type.h"
#define STR_SIZE 200
#define EXPR_SIZE 10
extern char * node_name[];
void pretty_print(A_NODE *);
void pprt_program(A_NODE *, int);
void pprt_statement(A_NODE *, int);
void pprt_statement_list(A_NODE *, int);
void pprt_expression(A_NODE *, int);
void pprt_A_TYPE(A_TYPE *, int);
void pprt_A_ID_LIST(A_ID *, int);
void pprt_A_ID(A_ID *, int);
void pprt_A_ID_NAME(A_ID *, int);
void pprt_STRING(char *, int);
void pprt_integer(int, int);

void pprint_node(A_NODE *,int);
void strshift(char *str, int n);
extern A_TYPE *int_type, *float_type, *char_type, *void_type, *string_type;

char str_buf[STR_SIZE] = {0};
char expr_str[STR_SIZE] = {0};
char expr_buf[EXPR_SIZE] = {0};

void strshift(char *str, int n)
{
	char buf[STR_SIZE] = {0};
	strcpy(buf, str_buf);
	strcpy(str_buf+n, buf); 
}

void pprint_node(A_NODE *node, int s)
{
	int i=0;
	if (!strcmp(node_name[node->name], "N_STMT_LIST"))
		while (s > i){
			printf("    ");
			i++;
		}
	switch(node->name) {
		case N_EXP_AMP :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"&");break;
		case N_EXP_STAR :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"*");break;
		case N_EXP_NOT :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"!");break;
		case N_EXP_PLUS :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"+");break;
		case N_EXP_MINUS :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"-");break;
		case N_EXP_MUL :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"* ");break;
		case N_EXP_DIV :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"/ ");break;
		case N_EXP_MOD :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"% ");break;
		case N_EXP_ADD :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"+ ");break;
		case N_EXP_SUB :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"- ");break;
		case N_EXP_LSS :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"< ");break;
		case N_EXP_GTR :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"> ");break;
		case N_EXP_LEQ :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"<= ");break;
		case N_EXP_GEQ :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,">= ");break;
		case N_EXP_NEQ :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"!= ");break;
		case N_EXP_EQL :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"== ");break;
		case N_EXP_AND :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"&& ");break;
		case N_EXP_OR :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"|| ");break;
		case N_EXP_ASSIGN :
			memset(expr_buf, 0, EXPR_SIZE);
			sprintf(expr_buf,"= ");break;
	}
		
}

void pretty_print(A_NODE *node)
{
	printf("\n=======ugly_print==========\n");
	pprt_program(node,0);
	printf("\b\b");
}

void pprt_program(A_NODE *node, int s)
{
	pprint_node(node,s);
	switch(node->name) {
		case N_PROGRAM:
			pprt_A_ID_LIST(node->clink, s);
			break;
		default :
			printf("****syntax tree error******");
	}
}

void pprt_expression(A_NODE *node, int s)
{
	pprint_node(node,s);

	switch(node->name) {
		case N_EXP_IDENT :
			pprt_A_ID_NAME(node->clink, s);
			break;
		case N_EXP_INT_CONST :
			pprt_integer(node->clink, s);
			break;
		case N_EXP_FLOAT_CONST :
			pprt_STRING(node->clink, s);
			break;
		case N_EXP_CHAR_CONST :
			pprt_integer(node->clink, s);
			break;
		case N_EXP_STRING_LITERAL :
			pprt_STRING(node->clink, s);
			break;
		case N_EXP_ARRAY :
			pprt_expression(node->llink, s);
//			printf("[");
			strcat(expr_str, "[");
			pprt_expression(node->rlink, s);
//			printf("]"); 
			strcat(expr_str, "]");
			break;
		case N_EXP_FUNCTION_CALL :
			pprt_expression(node->llink, s);
//			printf("() "); 
			strcat(expr_str, "() ");
			break;
		case N_EXP_AMP :
		case N_EXP_STAR :
		case N_EXP_NOT :
		case N_EXP_PLUS :
		case N_EXP_MINUS :
			pprt_expression(node->clink, s);
			break;
		case N_EXP_MUL :
		case N_EXP_DIV :
		case N_EXP_MOD :
		case N_EXP_ADD :
		case N_EXP_SUB :
		case N_EXP_LSS :
		case N_EXP_GTR :
		case N_EXP_LEQ :
		case N_EXP_GEQ :
		case N_EXP_NEQ :
		case N_EXP_EQL :
		case N_EXP_AND :
		case N_EXP_OR :
		case N_EXP_ASSIGN :
//			printf("( "); 
			strcat(expr_str, "( ");
			pprt_expression(node->llink, s);
//			printf(") "); 
			strcat(expr_str, ") ");
//			printf("%s", expr_buf); 
			strcat(expr_str, expr_buf);
//			printf("( "); 
			strcat(expr_str, "( ");
			pprt_expression(node->rlink, s);
//			printf(") "); 
			strcat(expr_str, ") ");
			break;
		default :
			printf("****syntax tree error******");
		}
}

void pprt_statement(A_NODE *node, int s)
{
	int i;
	pprint_node(node,s);
	
	switch(node->name) {
		case N_STMT_COMPOUND:
			printf("\n");
			for(i=0;i<s;i++)
				printf("    ");
			printf("{\n");

			if(node->llink)
				pprt_A_ID_LIST(node->llink, s+1);
			pprt_statement_list(node->rlink, s+1);
			
			printf("\n");
			for(i=0;i<s;i++)
				printf("    ");
			printf("}\n");
			break;
		case N_STMT_EMPTY:
			printf(";\n");			
			break;
		case N_STMT_EXPRESSION:
			memset(expr_str, 0, STR_SIZE);
			pprt_expression(node->clink, s);printf("%s", expr_str);
			printf("\b;\n");
			break;
		case N_STMT_IF_ELSE:
			printf("if (");
			memset(expr_str, 0, STR_SIZE);
			pprt_expression(node->llink, s);printf("%s", expr_str);
			printf("\b) ");
			pprt_statement(node->clink, s);
			for(i=0;i<s;i++)
				printf("    ");
			printf("else ");
			pprt_statement(node->rlink, s);
			break;
		case N_STMT_IF:
			printf("if (");
			memset(expr_str, 0, STR_SIZE);
			pprt_expression(node->llink, s);printf("%s", expr_str);
			printf("\b) ");
			pprt_statement(node->rlink, s);
			break;
		case N_STMT_WHILE:
			printf("while (");
			memset(expr_str, 0, STR_SIZE);
			pprt_expression(node->llink, s);printf("%s", expr_str);
			printf("\b) ");
			pprt_statement(node->rlink, s);
			if (node->rlink->name != N_STMT_COMPOUND)
				printf("\b;\n");
			break;
		case N_STMT_CONTINUE:
			printf("continue");
			printf(";\n");
			break;
		case N_STMT_BREAK:
			printf("break");
			printf(";\n");
			break;
		case N_STMT_RETURN:
			printf("return ");
			if(node->clink) {
				memset(expr_str, 0, STR_SIZE);
				pprt_expression(node->clink, s);printf("%s", expr_str);
			}
			printf("\b;\n");
			break;
		default :
			printf("****syntax tree error******");
	}
}

void pprt_statement_list(A_NODE *node, int s)
{
	pprint_node(node,s);

	switch(node->name) {
		case N_STMT_LIST:
			pprt_statement(node->llink, s);
			pprt_statement_list(node->rlink, s);
			break;
		case N_STMT_LIST_NIL:
			break;
		default :
			printf("****syntax tree error******");
	}
}

void pprt_integer(int a, int s)
{
	char buf[50] = {0};
	sprintf(buf, "%d ", a);
//	printf("%d ", a); 
	strcat(expr_str, buf);
}

void pprt_STRING(char *str, int s) {
	char buf[50] = {0};
	sprintf(buf, "%s ", str);
//	printf("%s ", str); 
	strcat(expr_str, buf);
}

extern char *type_kind_name[];

void pprt_A_TYPE(A_TYPE *t, int s)
{
	if (t==int_type)
		printf("int ");
	else if (t==float_type)
		printf("float ");
	else if (t==char_type)
		printf("char ",t->size);
	else if (t==void_type)
		printf("void ");
	else if (t->kind==T_NULL)
		;
	else if (t->prt)
		;
	else
		switch (t->kind) {
			case T_ENUM:
			t->prt=TRUE;
			printf("enum ");
			printf("{ ");
			pprt_A_ID_LIST(t->field,s);
			printf("} ");
			break;
		case T_POINTER:
			t->prt=TRUE;
//			printf("*"); 
			strshift(str_buf, 1); strncpy(str_buf, "*", 1);
			pprt_A_TYPE(t->element_type,s);
			break;
		case T_ARRAY:
			t->prt=TRUE;
			strshift(str_buf, 1); strncpy(str_buf, "(", 1);
			strcat(str_buf, ")");
			strcat(str_buf, "[");
//			printf("[");
			if (t->expr) {
				memset(expr_str, 0, STR_SIZE);
				pprt_expression(t->expr,s);
				strcat(str_buf, expr_str);
			}
			strcat(str_buf, "]");
//			printf("]");
			pprt_A_TYPE(t->element_type,s);
			break;
		case T_FUNC:
			t->prt=TRUE;
			strshift(str_buf, 1); strncpy(str_buf, "(", 1);
			strcat(str_buf, ")");
			strcat(str_buf, "() ");			
//			printf("() ");
			pprt_A_TYPE(t->element_type,s);
			printf("%s", str_buf);
			if (t->expr) {
				pprt_statement(t->expr,s);}
		}
}
void pprt_A_ID_LIST(A_ID *id, int s)
{
	int i;
	while (id) {
		if(id->kind != ID_ENUM_LITERAL) {
			for(i=0;i<s;i++)
				printf("    ");
			memset(str_buf, 0, STR_SIZE);
		}
		pprt_A_ID(id,s);
		id=id->link;
	}
}

extern char *id_kind_name[];
extern char *spec_name[];

void pprt_A_ID_NAME(A_ID *id, int s)
{
//	printf("%s ", id->name); 
	strcat(expr_str, id->name);
}

void pprt_A_ID(A_ID *id, int s)
{
	if (id->specifier == S_STATIC)
		printf("static ");
//	printf("%s ", id->name); 
	if (id->kind != ID_ENUM_LITERAL)
		sprintf(str_buf, "%s ", id->name);
	if (id->type) {
		pprt_A_TYPE(id->type,s);}
	if (id->kind == ID_ENUM_LITERAL)
		printf("%s ", id->name);
	else if (id->kind != ID_FUNC)
		printf("%s", str_buf);
	if (id->init) {
		printf("= ");
		if (id->kind==ID_ENUM_LITERAL) {
			memset(expr_str, 0, STR_SIZE);
			pprt_expression(id->init,s);printf("%s", expr_str); 
		}
	}
	if((id->kind != ID_FUNC) && (id->kind != ID_ENUM_LITERAL))
		printf(";\n");
}
