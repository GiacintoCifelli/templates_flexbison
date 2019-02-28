%output "parser.c"

%{
#include <stdio.h>
#define YYDEBUG	1

int yylex();
void yyerror(char *msg);

%}

%%

result: ;

%%

extern int chars;
extern int words;
extern int lines;

extern int yydebug;

int main(int argc, char *argv[])
{
	yydebug = 1;
	yyparse();
	printf("lines: %d\nwords: %d\nchars: %d\n", lines, words, chars);
}

void yyerror(char *msg)
{
	printf("error: %s\n", msg);
}
