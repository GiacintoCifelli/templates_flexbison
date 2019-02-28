%option outfile="mpd_lexer.c"
%option header-file="mpd_lexer.h"
%option noyywrap
%option nounput
%option noinput
%option nodefault
%option reentrant
%option bison-bridge

%{
typedef struct {
	void *scaninfo;
	int chars;
	int words;
	int lines;
	int retlen;
} _stats;
#include "mpd_parser.h"
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
