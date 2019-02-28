%output "pid_parser.c"

%{
#include <stdio.h>
#define YYDEBUG	1

int yylex();
void yyerror(char *msg);

%}

%%

result: ;

%%

int main(int argc, char *argv[])
{
	yydebug = 1;
	yyparse();
}

void yyerror(char *msg)
{
	printf("error: %s\n", msg);
}

int yylex() {
	return 0;
}
