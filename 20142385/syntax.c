#include "type.h"
#include "y.tab.h"
extern char *yytext;

A_TYPE *int_type, *char_type, *void_type, *float_type, *string_type;
A_NODE *root;
A_ID *current_id=NIL;
int syntax_err=0;
int line_no=1;
int current_level=0;

A_NODE *makeNode (NODE_NAME,A_NODE *,A_NODE *,A_NODE *);
A_NODE *makeNodeList (NODE_NAME,A_NODE *,A_NODE *);
A_ID *makeIdentifier(char *);
A_ID *makeDummyIdentifier();
A_TYPE *makeType(T_KIND);
A_SPECIFIER *makeSpecifier(A_TYPE *,S_KIND);
A_ID *searchIdentifier(char *,A_ID *);
A_ID *searchIdentifierAtCurrentLevel(char *,A_ID *);
A_SPECIFIER *updateSpecifier(A_SPECIFIER *, A_TYPE *, S_KIND);
void setDefaultSpecifier(A_SPECIFIER *);
A_ID *linkDeclaratorList(A_ID *,A_ID *) ;
A_ID *getIdentifierDeclared(char *);
A_TYPE *getTypeOfStructOrEnumRefIdentifier(T_KIND,char *,ID_KIND);
A_ID *setDeclaratorInit(A_ID *,A_NODE *);
A_ID *setDeclaratorKind(A_ID *,ID_KIND);
A_ID *setDeclaratorType(A_ID *,A_TYPE *);
A_ID *setDeclaratorElementType(A_ID *,A_TYPE *);
A_ID *setDeclaratorTypeAndKind(A_ID *,A_TYPE *,ID_KIND);
A_ID *setDeclaratorListSpecifier(A_ID *,A_SPECIFIER *);
A_ID *setFunctionDeclaratorSpecifier(A_ID *, A_SPECIFIER *);
A_ID *setFunctionDeclaratorBody(A_ID *, A_NODE *);
A_TYPE *setTypeNameSpecifier(A_TYPE *, A_SPECIFIER *);
A_TYPE *setTypeElementType(A_TYPE *,A_TYPE *);
A_TYPE *setTypeField(A_TYPE *,A_ID *);
A_TYPE *setTypeExpr(A_TYPE *,A_NODE *);
A_TYPE *setTypeAndKindOfDeclarator(A_TYPE *,ID_KIND,A_ID *);
A_TYPE *setTypeStructOrEnumIdentifier(T_KIND,char *,ID_KIND);
BOOLEAN isNotSameType(A_TYPE *, A_TYPE *);BOOLEAN isPointerOrArrayType(A_TYPE *);
void syntax_error();
void initialize();

// make new node for syntax tree
A_NODE *makeNode (NODE_NAME n, A_NODE *a, A_NODE *b, A_NODE *c)
{
	A_NODE *m;
	m = (A_NODE*)malloc(sizeof(A_NODE));
	m->name=n;
	m->llink=a;
	m->clink=b;
	m->rlink=c;
	m->type=NIL;
	m->line=line_no;
	m->value=0;
	return (m);
}

A_NODE *makeNodeList (NODE_NAME n, A_NODE *a, A_NODE *b)
{
	A_NODE *m,*k;
	k=a;
	while (k->rlink)
		k=k->rlink;
	m = (A_NODE*)malloc(sizeof(A_NODE));
	m->name=k->name;
	m->llink=NIL;
	m->clink=NIL;
	m->rlink=NIL;
	m->type=NIL;
	m->line=line_no;
	m->value=0;
	k->name=n;
	k->llink=b;
	k->rlink=m;
	return(a);
}

// make new declarator for identifier
A_ID *makeIdentifier(char *s)
{
	A_ID *id;
	id = (A_ID*)malloc(sizeof(A_ID));
	id->name = s;
	id->kind = 0;
	id->specifier = 0;
	id->level = current_level;
	id->address = 0;
	id->init = NIL;
	id->type = NIL;
	id->link = NIL;
	id->line = line_no;
	id->value=0;
	id->prev = current_id;
	current_id = id;
	// ...
	// 심볼테이블을 하나 추가
	// ...
	// ...
	return(id);
}

// make new declarator for dummy identifier
A_ID *makeDummyIdentifier()
{
	A_ID *id;
	id = (A_ID*)malloc(sizeof(A_ID));
	id->name = "";
	id->kind = 0;
	id->specifier = 0;
	id->level = current_level;
	id->address = 0;
	id->init = NIL;
	id->type = NIL;
	id->link = NIL;
	id->line = line_no;
	id->value=0;
	id->prev =0;
	return(id);
}

// make new type
A_TYPE *makeType(T_KIND k)
{
	A_TYPE *t;
	t = (A_TYPE*)malloc(sizeof(A_TYPE));
	t->kind = k;
	t->size=0;
	t->local_var_size=0;
	t->element_type = NIL;
	t->field = NIL;
	t->expr = NIL;
	t->check=FALSE;
	t->prt=FALSE;
	t->line=line_no;
	return(t);
}

// make new specifier
A_SPECIFIER *makeSpecifier(A_TYPE *t,S_KIND s)
{
	A_SPECIFIER *p;
	p = (A_SPECIFIER*)malloc(sizeof(A_SPECIFIER));
	p->type = t;
	p->stor=s;
	p->line=line_no;
	return(p);
}

A_ID *searchIdentifier(char *s, A_ID *id)
{
	while (id) {
		if (strcmp(id->name,s)==0)
			break;
		id=id->prev;
	}
	return(id);
}

A_ID *searchIdentifierAtCurrentLevel(char *s,A_ID *id)
{
	// 심볼테이블 목록의 id 로부터 시작해서 현재 level 에서 선언된 이름 탐사
	while (id) {
		if (id->level < current_level) {
			return (NIL);
		}	
		if (strcmp(id->name,s)==0)
			break;
		id = id->prev;
	// ...
	//...
	}
	return(id);
}

// set default specifier
void setDefaultSpecifier(A_SPECIFIER *p)
{
	A_TYPE *t;
	if (p->type==NIL)
		p->type=int_type;
	if (p->stor==S_NULL)
		p->stor=S_AUTO;
}

// specifier merge & update
A_SPECIFIER *updateSpecifier(A_SPECIFIER *p, A_TYPE *t, S_KIND s)
{
	if (t)
		if ( p->type)
			if (p->type==t)
				;
			else
				syntax_error(24);
		else
			p->type=t;
	if (s) {
		if (p->stor)
			if(s==p->stor)
				;
			else
				syntax_error(24);
		else
			p->stor=s;
	}
	return (p);
}

// link two declaraor list id1 & id2
A_ID *linkDeclaratorList(A_ID *id1, A_ID *id2)
{
	A_ID * m = id1;
	if (id1==NIL) 
		return(id2);
	while(m->link)
		m=m->link;
	m->link=id2;
	return (id1);
}

// check if identifier is already declared in primary expression
A_ID *getIdentifierDeclared(char *s){
	A_ID *id;
	id=searchIdentifier(s,current_id);
	if(id==NIL)
		syntax_error(13,s);
	return(id);
}

// get type of struct id
A_TYPE * getTypeOfStructOrEnumRefIdentifier(T_KIND k,char *s,ID_KIND kk)
{
	A_TYPE *t;
	A_ID * id;
	id=searchIdentifier(s,current_id);
	if (id && id->kind==kk && id->type->kind==k)
		return(id->type);
	syntax_error(11,s);
	t=makeType(k);
	id=makeIdentifier(s);
	id->kind=kk;
	id->type=t;
	return(int_type);
}

// set declarator init (expression tree)
A_ID *setDeclaratorInit(A_ID *id, A_NODE *n)
{
	id->init=n;
	return(id);
}

// set declarator kind
A_ID *setDeclaratorKind(A_ID *id, ID_KIND k)
{
	A_ID *a;
	a=searchIdentifierAtCurrentLevel(id->name,id->prev);
	if (a)
		syntax_error(12,id->name);
	id->kind=k;
	return(id);
}

// set declarator type
A_ID *setDeclaratorType(A_ID *id, A_TYPE *t)
{
	id->type=t;
	return(id);
}

// set declarator type (or element type)
A_ID *setDeclaratorElementType(A_ID *id, A_TYPE *t)
{
	A_TYPE *tt;
	
	if (id->type==NIL)
		id->type = t;
	else {
		tt = id->type;
		while (tt->element_type)
			tt = tt->element_type;
		tt->element_type = t;
	}
	// 심볼테이블 id 의 가장 기본 타입을 t 로 지정
	// ...
	// ...
	return (id);
}

// set declarator element type and kind
A_ID *setDeclaratorTypeAndKind(A_ID *id, A_TYPE *t,ID_KIND k)
{
	id=setDeclaratorElementType(id,t);
	id=setDeclaratorKind(id,k);
	return(id);
}

// check function declarator and return type
A_ID *setFunctionDeclaratorSpecifier(A_ID *id, A_SPECIFIER *p)
{
	A_ID *a;
	// check storage class
	if (p->stor)
		syntax_error(25);
	setDefaultSpecifier(p);
	
	if (id->type->kind != T_FUNC) {
		syntax_error(21);
		return (id);
	} 
	else {
		id = setDeclaratorElementType(id, p->type);
		id->kind = ID_FUNC;
	}
	// 심볼테이블 id 의 이름이 함수이름으로 올바른지 검사 하고 이름의 종류를
	// ID_FUNC 으로 지정하고 리턴 타입을 지정한다
	// ...
	// ...
	// check redeclaration
	a=searchIdentifierAtCurrentLevel(id->name,id->prev);
	if (a && (a->kind!=ID_FUNC || a->type->expr))
		syntax_error(12,id->name);
	return(id);
}

A_ID *setFunctionDeclaratorBody(A_ID *id, A_NODE *n)
{
	id->type->expr=n;
	return(id);
}

// set declarator_list type and kind based on storage class
A_ID *setDeclaratorListSpecifier(A_ID *id, A_SPECIFIER *p)
{
	A_ID *a;
	setDefaultSpecifier(p);
	a=id;

	while (a) {
		if(strlen(a->name)&&searchIdentifierAtCurrentLevel(a->name,a->prev))
			syntax_error(12,a->name);

		a = setDeclaratorElementType(a, p->type);

		if (a->type->kind == T_FUNC)
			a->kind = ID_FUNC;
		else 
			a->kind = ID_VAR;
		
		a->specifier =  p->stor;
		if (a->specifier == S_NULL)
			a->specifier = S_AUTO;
		else
			a->specifier =  p->stor;
		
		// 심볼테이블 a의 이름이 중복선언되었는지 검사하고 그 타임을 지정하고
		// 그 이름의 종류를 결정
		// ...
		// ...
		a=a->link;
	}
	return(id);
}

// set type name specifier
A_TYPE *setTypeNameSpecifier(A_TYPE *t, A_SPECIFIER *p)
{
	// check storage class in type name
	if (p->stor)
		syntax_error(20);
	setDefaultSpecifier(p);
	t=setTypeElementType(t,p->type);
	return(t);
}

// set type element type
A_TYPE *setTypeElementType(A_TYPE *t, A_TYPE *s)
{
	A_TYPE *q;
	if (t==NIL)
		return(s);
	q=t;
	while (q->element_type)
		q=q->element_type;
	q->element_type=s;
	return(t);
}

// set type field
A_TYPE *setTypeField(A_TYPE *t, A_ID *n)
{
	t->field=n;
	return(t);
}

// set type initial value (expression tree)
A_TYPE *setTypeExpr(A_TYPE *t, A_NODE *n)
{
	t->expr=n;
	return(t);
}

// set type of struct iIdentifier
A_TYPE *setTypeStructOrEnumIdentifier(T_KIND k, char *s, ID_KIND kk)
{
	A_TYPE *t;
	A_ID *id, *a;
	// check redeclaration or forward declaration
	a=searchIdentifierAtCurrentLevel(s,current_id);
	if (a) syntax_error(12,s);
	// make a new struct (or enum) identifier
	id=makeIdentifier(s);
	t=makeType(k);
	id->type=t;
	id->kind=kk;
	return(t);
}

// set type and kinf of identifier
A_TYPE *setTypeAndKindOfDeclarator(A_TYPE *t, ID_KIND k, A_ID *id)
{
	if (searchIdentifierAtCurrentLevel(id->name,id->prev))
		syntax_error(12,id->name);
	id->type=t;
	id->kind=k;
	return(t);
}
void initialize()
{
	// primitive data types
	int_type=setTypeAndKindOfDeclarator(makeType(T_ENUM),ID_TYPE,makeIdentifier("int"));
	float_type=setTypeAndKindOfDeclarator(makeType(T_ENUM),ID_TYPE,makeIdentifier("float"));
	char_type= setTypeAndKindOfDeclarator(makeType(T_ENUM),ID_TYPE,makeIdentifier("char"));
	void_type= setTypeAndKindOfDeclarator(makeType(T_VOID),ID_TYPE,makeIdentifier("void"));
	string_type=setTypeElementType(makeType(T_POINTER),char_type);
	int_type->check=TRUE;
	float_type->check=TRUE;
	char_type->check=TRUE;
	void_type->check=TRUE;
	string_type->check=TRUE;
}
void syntax_error(int i,char *s)
{
	syntax_err++;
	printf("line %d: syntax error: ", line_no);
	switch (i) {
		case 11: printf("illegal referencing struct or union identifier %s",s);break;
		case 12: printf("redeclaration of identifier %s",s); break;
		case 13: printf("undefined identifier %s",s); break;
		case 14: printf("illegal type specifier in formal parameter"); break;case 20: printf("illegal storage class in type specifiers"); break;
		case 21: printf("illegal function declarator"); break;
		case 22: printf("conflicting parameter type in prototype function %s",s);break;
		case 23: printf("empty parameter name"); break;
		case 24: printf("illegal declaration specifiers"); break;
		case 25: printf("illegal function specifiers"); break;
		case 26: printf("illegal or conflicting return type in prototype function%s",s);
		break;
		case 31: printf("undefined type for identifier %s",s); break;
		case 32: printf("incomplete forward reference for identifier %s",s);
		break;
		default: printf("unknown"); break;
	}
	if (strlen(yytext)==0)
		printf(" at end\n");
	else
		printf(" near %s\n", yytext);
}
