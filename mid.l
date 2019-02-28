%option outfile="mid_lexer.c"
%option noyywrap
%option nounput
%option noinput

%{
#include "mid_parser.h"
%}

%%

[A-Za-z]+	{ return WORD; }
\n		{ return EOL; }
.		{ return CHAR; }

%%
