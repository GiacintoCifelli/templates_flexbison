%output "v2_parser.c"
%defines

%{
#include <stdio.h>
#define YYDEBUG	1

int yylex();
void yyerror(char *msg);

extern int yyleng;

int credit = 0;
void add(int coin);

%}

%token A B C D E S

%%

idle:				{ printf("idling\n"); }
 | idle cancel			{ printf("-\n"); }
 | idle vend			{ printf("+\n"); }
 | idle coin
 | idle S			{ add(0); }
 ;
coin: A				{ add(100); }
 | B				{ add(50); }
 | C				{ add(25); }
 ;
vend: D				{
					/* here it would be possible to check (credit>100) and return change */
					if(credit==100) {
						printf("sold!\n");
						credit = 0;
					} else yyerror("insufficient credit");
				}
 ;
cancel: E			{ if(credit) printf("returning all coins (%d)", credit); else yyerror("nothing to cancel!"); credit=0; }

%%

void add(int coin) {
	if( credit+coin > 100 ) {
		/* here it would be possible to just return the extra or do nothing */
		printf("excessive credit, returning the last %d coin\n", coin);
		return;
	}
	credit += coin;
	printf("credit: %3d\n", credit);
}

int main(int argc, char *argv[])
{
	printf("v2 state machine, designed from BNF, using the same input from lexer (supposedly, buttons)\n");
	printf("100 cents for a 'soda', 'cancel' to return money, exact change, accepted '25', '50', '75', '100'\n");
	printf("it would be easy to return change in this design (see code comments), but keep equivalent behavior\n");
	yydebug = 0;
	yyparse();
}

void yyerror(char *msg)
{
	printf("error: %s\n", msg);
	yyclearin;
}
