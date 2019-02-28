%name-prefix "cust_"
%output "mir_parser.c"
%defines

%{
#include <stdio.h>
#define YYDEBUG	1

int cust_lex();
void cust_error(char *msg);

int chars = 0;
int words = 0;
int lines = 0;

extern int cust_leng;

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
 	chars += cust_leng; }
;

%%

int main(int argc, char *argv[])
{
	cust_debug = 1;
	cust_parse();
	printf("\n\nlines: %d\nwords: %d\nchars: %d\n", lines, words, chars);
}

void cust_error(char *msg)
{
	printf("error: %s\n", msg);
}
