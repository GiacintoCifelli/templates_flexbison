%option outfile="lexer.c"
%option noyywrap
%option nounput
%option noinput

%{
	int chars = 0;
	int words = 0;
	int lines = 0;
%}

%%

[A-Za-z]+	{ words++; chars += strlen(yytext); }
\n		{ chars++; lines++; }
.		{ chars++; }

%%

/*
#include <stdio.h>

int main(int argc, char *argv[])
{
	yylex (); // < note that this will only return once with the entire input parsed: single state
	printf("lines: %d\nwords: %d\nchars: %d\n", lines, words, chars);
}
*/