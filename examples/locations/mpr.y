%output "mpr_parser.c"
%defines
%define api.pure
%parse-param { _stats *stats }
%lex-param { SCANINFO }
%debug
%name-prefix "cust_"
%locations

%{
#include <stdarg.h>
#include <stdio.h>

#define SCANINFO	stats->scaninfo

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
#include "mpr_lexer.h"

int cust_lex(YYSTYPE *yylvalp, YYLTYPE *yylocp, void *scaninfo);
void cust_error(YYLTYPE *yylocp, _stats *stats, char *msg, ...);
%}

%code requires {
/* this goes to the header file, so that also the lexer can access the modified structures */

typedef struct {
  int first_line;
  int first_column;
  int last_line;
  int last_column;
  char *cur_file;
} YYLTYPE;
# define YYLTYPE_IS_DECLARED 1

# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
          (Current).cur_file     = YYRHSLOC (Rhs, 1).cur_file;          \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
          (Current).cur_file     = NULL;                                \
        }                                                               \
    while (0)

}

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
	char *cur_file = "bogus";
	_stats stats = {NULL, cur_file, 0, 0, 0};
	cust_lex_init_extra(&stats, &stats.scaninfo);
	//cust_set_in(stdin, stats);
	//cust_debug = 1;
	cust_parse(&stats);
	cust_lex_destroy(stats.scaninfo);
	printf("lines: %d\nwords: %d\nchars: %d\n", stats.lines, stats.words, stats.chars);
}

void cust_error(YYLTYPE *yylocp, _stats *stats, char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	printf("error: ");
	if(yylocp->first_line)
		printf("(l:%d,c:%d)->(l:%d,c:%d)", yylocp->first_line, yylocp->first_column, yylocp->last_line, yylocp->last_column);
//		printf("file:%s, (l:%d,c:%d)->(l:%d,c:%d)", yylloc.cur_file, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column);
	vprintf(msg, args);
	printf("\n");
	printf("current stats: lines: %d\nwords: %d\nchars: %d\n", stats->lines, stats->words, stats->chars);
	va_end(args);
}
