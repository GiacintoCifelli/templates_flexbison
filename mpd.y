%output "mpd_parser.c"
%defines
%define api.pure
%parse-param { _stats *stats }
%lex-param { SCANINFO }
%debug

%{
#include <stdio.h>

#define SCANINFO	stats->scaninfo

typedef struct {
	void *scaninfo;
	int chars;
	int words;
	int lines;
	int retlen;
} _stats;
#include "mpd_parser.h"
#include "mpd_lexer.h"

int yylex(YYSTYPE *yylvalp, void* scaninfo);
//int yylex(YYSTYPE *yylvalp, YYLTYPE *yylocp, void *scaninfo); // with locations
void yyerror(_stats *stats, char *msg);
//void yyerror(YYLTYPE *yylocp, _stats *stats, char *msg); // with locations
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
	yylex_init_extra(&stats, &stats.scaninfo);
	//yyset_in(stdin, stats);
	yydebug = 1;
	yyparse(&stats);
	yylex_destroy(stats.scaninfo);
	printf("lines: %d\nwords: %d\nchars: %d\n", stats.lines, stats.words, stats.chars);
}

void yyerror(_stats *stats, char *msg)
{
	printf("error: %s\n", msg);
}
