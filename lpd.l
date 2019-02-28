%option outfile="lpd_lexer.c"
%option noyywrap
%option nounput
%option noinput
%option nodefault
%option reentrant

%{
struct _stats {
	int chars;
	int words;
	int lines;
};
%}
%option extra-type="struct _stats *"

%%

%{
	struct _stats *stats = yyextra;
%}


[A-Za-z]+	{ stats->words++; stats->chars += yyleng; }
\n		{ stats->chars++; stats->lines++; }
.		{ stats->chars++; }

%%

#include <stdio.h>

int main(int argc, char *argv[])
{
	yyscan_t scanner;
	struct _stats stats = {0, 0, 0};
	yylex_init_extra(&stats, &scanner);
	yyset_in(stdin, scanner);
	yylex(scanner); // < note that this will only return once with the entire input parsed: single state
	printf("lines: %d\nwords: %d\nchars: %d\n", stats.lines, stats.words, stats.chars);
	yylex_destroy(scanner);
}
