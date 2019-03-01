%option outfile="lpr_lexer.c"
%option noyywrap
%option nounput
%option noinput
%option nodefault
%option reentrant
%option prefix="cust_"

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
	yyscan_t scanner;			// << this remains yy
	struct _stats stats = {0, 0, 0};
	cust_lex_init_extra(&stats, &scanner);
	cust_set_in(stdin, scanner);
	cust_lex(scanner); // < note that this will only return once with the entire input parsed: single state
	printf("lines: %d\nwords: %d\nchars: %d\n", stats.lines, stats.words, stats.chars);
	cust_lex_destroy(scanner);
}
