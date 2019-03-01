%output "ppr_parser.c"
%defines
%define api.pure
%parse-param { void *info }
%lex-param { INFO }
%debug
%name-prefix "cust_"

%{
#define INFO  info
#include <stdio.h>
#include "ppr_parser.h"

int cust_lex(YYSTYPE *yylvalp, void *scaninfo);
//int cust_lex(YYSTYPE *yylvalp, YYLTYPE *yylocp); // with locations
void cust_error(void *info, char *msg);
//void cust_error(YYLTYPE *yylocp, void *info, char *msg); // with locations
%}

%%

result: ;

%%

int main(int argc, char *argv[])
{
	cust_debug = 1;
	void *p = NULL;
	cust_parse(&p);
}

void cust_error(void *info, char *msg)
{
	printf("error: %s\n", msg);
}

int cust_lex(YYSTYPE *yylvalp, void *scaninfo) {
	printf("\n\n%p\n\n", yylvalp);
	return 0;
}
