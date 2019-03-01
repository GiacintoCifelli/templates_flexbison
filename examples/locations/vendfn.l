%option outfile="vendfn_lexer.c"
%option header-file="vendfn_lexer.h"
%option noyywrap
%option nounput
%option noinput
%option nodefault

%{
#include "vendfn_parser.h"

int yycolumn = 1;

#define YY_USER_ACTION \
	yylloc.cur_file = cur_file; \
	yylloc.first_line = yylloc.last_line = yylineno; \
	yylloc.first_column = yycolumn; \
	yycolumn += yyleng; \
	yylloc.last_column = yycolumn-1;
%}

%%

100[ \t]*\n		{ yycolumn = 1; return A; }
50[ \t]*\n		{ yycolumn = 1; return B; }
25[ \t]*\n		{ yycolumn = 1; return C; }
soda[ \t]*\n		{ yycolumn = 1; return D; }
cancel[ \t]*\n		{ yycolumn = 1; return E; }
status[ \t]*\n		{ yycolumn = 1; return S; }

[ \t\r\n]+		/* ignore blanks*/
.			/* ignore anything else */

%%
