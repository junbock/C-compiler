#include "type.h"
extern int syntax_err;
extern A_NODE *root;
extern syntax_err;
void initialize();
void parse();
void print_ast();
void pretty_print();
extern FILE *yyin;

void main(int argc, char *argv[])
{
	if (argc<2){
		printf("source file not given\n");
		exit(1);
	}
	if ((yyin=fopen(argv[argc-1],"r"))==NULL){
		printf("can not open input file: %s\n",argv[argc-1]);
		exit(1);
	}
	printf("start syntax analysis\n");
	initialize();
	yyparse();
	if (syntax_err)
		exit(1);
	print_ast(root);
	pretty_print(root);
	exit(0);
}
