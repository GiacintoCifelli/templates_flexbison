%output "pir_parser.c"
%debug
%name-prefix "cust_"

%{
#include <stdio.h>

int cust_lex();
void cust_error(char *msg);

%}

%%

result: ;

%%

int main(int argc, char *argv[])
{
	cust_debug = 1;
	cust_parse();
}

void cust_error(char *msg)
{
	printf("error: %s\n", msg);
}

int cust_lex() {
	return 0;
}
