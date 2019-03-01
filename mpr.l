%option outfile="mpr_lexer.c"
%option header-file="mpr_lexer.h"
%option noyywrap
%option nounput
%option noinput
%option nodefault
%option reentrant
%option bison-bridge
%option prefix="cust_"

%{
// include a header file
typedef struct {
	void *scaninfo;
	int chars;
	int words;
	int lines;
	int retlen;
} _stats;
#include "mpr_parser.h"
%}
%option extra-type="_stats *"

%%

%{
	_stats *stats = yyextra;
%}


[A-Za-z]+	{ stats->retlen = yyleng; return WORD; }
\n		{ return EOL; }
.		{ return CHAR; }

%%
