%output "ppd_parser.c"
%defines
%define api.pure
%parse-param { void *info }
%lex-param { INFO }
%debug

%{
#define INFO  info
#include <stdio.h>
#include "ppd_parser.h"

int yylex(YYSTYPE *yylvalp, void *scaninfo);
//int yylex(YYSTYPE *yylvalp, YYLTYPE *yylocp); // with locations
void yyerror(void *info, char *msg);
//void yyerror(YYLTYPE *yylocp, void *info, char *msg); // with locations
%}

%%

result: ;

%%

int main(int argc, char *argv[])
{
	yydebug = 1;
	void *p = NULL;
	yyparse(&p);
}

void yyerror(void *info, char *msg)
{
	printf("error: %s\n", msg);
}

int yylex(YYSTYPE *yylvalp, void *scaninfo) {
	printf("\n\n%p\n\n", yylvalp);
	return 0;
}
