%output "mpr_parser.c"
%defines
%define api.pure
%parse-param { _stats *stats }
%lex-param { SCANINFO }
%debug
%name-prefix "cust_"

%{
#include <stdio.h>

#define SCANINFO	stats->scaninfo

// include a header file
typedef struct {
	void *scaninfo;
	int chars;
	int words;
	int lines;
	int retlen;
} _stats;
#include "mpr_parser.h"
#include "mpr_lexer.h"

int cust_lex(YYSTYPE *yylvalp, void* scaninfo);
//int cust_lex(YYSTYPE *yylvalp, YYLTYPE *yylocp, void *scaninfo); // with locations
void cust_error(_stats *stats, char *msg);
//void cust_error(YYLTYPE *yylocp, _stats *stats, char *msg); // with locations
%}

%token EOL CHAR WORD

%%

result:
 | result EOL {
	stats->chars++;
	stats->lines++; }
 | result CHAR {
	stats->chars++; }
 | result WORD {
 	stats->words++;
 	stats->chars += stats->retlen; }
;

%%

int main(int argc, char *argv[])
{
	_stats stats = {NULL, 0, 0, 0};
	cust_lex_init_extra(&stats, &stats.scaninfo);
	//cust_set_in(stdin, stats);
	cust_debug = 1;
	cust_parse(&stats);
	cust_lex_destroy(stats.scaninfo);
	printf("lines: %d\nwords: %d\nchars: %d\n", stats.lines, stats.words, stats.chars);
}

void cust_error(_stats *stats, char *msg)
{
	printf("error: %s\n", msg);
}
