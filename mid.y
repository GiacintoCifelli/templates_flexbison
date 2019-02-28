%output "mid_parser.c"
%defines

%{
#include <stdio.h>
#define YYDEBUG	1

int yylex();
void yyerror(char *msg);

int chars = 0;
int words = 0;
int lines = 0;

extern int yyleng;

%}

%token EOL CHAR WORD

%%

result:
 | result EOL {
	chars++;
	lines++; }
 | result CHAR {
	chars++; }
 | result WORD {
 	words++;
 	chars += yyleng; }
;

%%

extern int yydebug;

int main(int argc, char *argv[])
{
	yydebug = 1;
	yyparse();
	printf("\n\nlines: %d\nwords: %d\nchars: %d\n", lines, words, chars);
}

void yyerror(char *msg)
{
	printf("error: %s\n", msg);
}
