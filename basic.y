%output "parser.c"

%{
	#include <stdio.h>

int yylex();
void yyerror(char *msg);

%}

%%

result: ;

%%

extern int chars;
extern int words;
extern int lines;

int main(int argc, char *argv[])
{
	yyparse();
	printf("lines: %d\nwords: %d\nchars: %d\n", lines, words, chars);
}

void yyerror(char *msg)
{
	printf("error: %s\n", msg);
}
