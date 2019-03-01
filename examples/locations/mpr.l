%option outfile="mpr_lexer.c"
%option header-file="mpr_lexer.h"
%option noyywrap
%option nounput
%option noinput
%option nodefault
%option reentrant
%option bison-bridge
%option bison-locations
%option prefix="cust_"

%{
// include a header file
typedef struct {
	void *scaninfo;
	char *cur_file;
	int chars;
	int words;
	int lines;
	int retlen;
	int column;
} _stats;
#include "mpr_parser.h"

//#define yycolumn stats->column

#define YY_USER_ACTION \
	yylloc->cur_file = stats->cur_file; \
	yylloc->first_line = yylloc->last_line = yylineno; \
	yylloc->first_column = yycolumn; \
	yycolumn += yyleng; \
	yylloc->last_column = yycolumn-1;

%}
%option extra-type="_stats *"

%%

%{
	_stats *stats = yyextra;
	yycolumn = 1;
%}


[A-Za-z]+	{ stats->retlen = yyleng; return WORD; }
\n		{ yycolumn = 1; return EOL; }
.		{ return CHAR; }

%%
