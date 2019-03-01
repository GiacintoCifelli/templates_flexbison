%output "vending_parser.c"
%defines

%{
#include <stdio.h>
#define YYDEBUG	1

int yylex();
void yyerror(char *msg);

extern int yyleng;

%}

%token A B C D E S

%%

idle:       			{ printf("idling\n"); }
 | idle e   			{ printf("-\n"); }
 | idle a d 			{ printf("+\n"); }
 | idle D			{ yyerror("insufficient credit"); }
 | idle S			{ printf("credit: 0\n"); }
 ;
a: A    			{ printf("reached 100\n"); }
 | b B  			{ printf("reached 100\n"); }
 | f C  			{ printf("reached 100\n"); }
 | a error			{ yyerror("invalid sum: last coin returned"); }
 | a S				{ printf("credit: 100\n"); }
 ;
b: B    			{ printf("reached  50\n"); }
 | c c  			{ printf("reached  50\n"); }
 | b D  			{ yyerror("insufficient credit"); }
 | b error			{ yyerror("invalid sum:  last coin returned"); }
 | b S				{ printf("credit:  50\n"); }
 ;
c: C    			{ printf("reached  25\n"); }
 | c D  			{ yyerror("insufficient credit"); }
 | c error			{ yyerror("invalid sum:  last coint returned"); }
 | c S				{ printf("credit:  25\n"); }
 ;
d: D    			{ printf("sold!\n"); }
 ;
e: E    			{ yyerror("nothing to cancel"); }
 | a E 				{ printf("canceled\n"); }
 | b E 				{ printf("canceled\n"); }
 | c E 				{ printf("canceled\n"); }
 | f E 				{ printf("canceled\n"); }
 ;
f: b C 				{ printf("reached 75\n"); }
 | c B 				{ printf("reached 75\n"); }
 | f D  			{ yyerror("insufficient credit"); }
 | f error			{ yyerror("invalid sum:  last coin returned"); }
 | f S				{ printf("credit:  75\n"); }
 ;

%%

int main(int argc, char *argv[])
{
	printf("vending state machine brutal backporting to BNF\n");
	printf("100 cents for a 'soda', 'cancel' to return money, exact change, accepted '25', '50', '75', '100'\n");
	yydebug = 0;
	yyparse();
}

void yyerror(char *msg)
{
	printf("error: %s\n", msg);
	yyclearin;
}

/********************************************************************************************************

FSM, state-machine diagram, corresponding to the Context-free-grammar above, in Backus-Naur Format BNF


        c=.25$ -------------------> f=.75$
        ^     \                    ^    |
        |      \                  /     |
        |       \                /      |
        |        \              /       |
        |         \            /        |
        |          \          /         |
        |           \        /          |
        |            v      /           |
        |             b=.50$            |
        |            ^     \            |
        |           /       \           |
        |          /         \          |
        |         /           \         |
        |        /             \        |
        |       /               \       |
        |      /                 \      |
        |     /                   \     |
        |    /                     \    |
        |   /                       v   v
        idle ----------------------> a=1$ -----> d=sell (pseudostate) --> back to idle

                e=cancel (from any except d, pseudostate) --> back to idle

		error: invalid input in this state machine: rejected and no change of state

********************************************************************************************************/
