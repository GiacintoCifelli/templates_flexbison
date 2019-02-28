%option outfile="mir_lexer.c"
%option noyywrap
%option nounput
%option noinput
%option nodefault
%option prefix="cust_"

%{
#include "mir_parser.h"
%}

%%

[A-Za-z]+	{ return WORD; }
\n		{ return EOL; }
.		{ return CHAR; }

%%
