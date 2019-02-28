%option outfile="lir_lexer.c"
%option noyywrap
%option nounput
%option noinput
%option nodefault
%option prefix="cust_"

%{
	int chars = 0;
	int words = 0;
	int lines = 0;
%}

%%

[A-Za-z]+	{ words++; chars += yyleng; }
\n		{ chars++; lines++; }
.		{ chars++; }

%%

#include <stdio.h>

int main(int argc, char *argv[])
{
	cust_lex(); // < note that this will only return once with the entire input parsed: single state
	printf("lines: %d\nwords: %d\nchars: %d\n", lines, words, chars);
}
