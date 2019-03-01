%option outfile="v2_lexer.c"
%option noyywrap
%option nounput
%option noinput
%option nodefault

%{
#include "v2_parser.h"
%}

%%

100[ \t]*\n		{ return A; }
50[ \t]*\n		{ return B; }
25[ \t]*\n		{ return C; }
soda[ \t]*\n		{ return D; }
cancel[ \t]*\n		{ return E; }
status[ \t]*\n		{ return S; }

[ \t\r\n]+		/* ignore blanks*/
.			/* ignore anything else */

%%
