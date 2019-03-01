%output "vendfn_parser.c"
%defines
%locations
%debug

%{
#include <stdarg.h>
#include <stdio.h>
#include "vendfn_parser.h"
#include "vendfn_lexer.h"

void yyerror(char *msg, ...);
void lyyerror(YYLTYPE t, char *msg, ...); /* use to report parameter position $1, $2, result: $@ */

char *cur_file = "bogus";

%}

%code requires {
/* this goes to the header file, so that also the lexer can access the modified structures */

extern char *cur_file;

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

%token A B C D E S

%%

idle:       			{ printf("idling\n"); }
 | idle e   			{ printf("-\n"); }
 | idle a d 			{ printf("+\n"); }
 | idle D			{ lyyerror(@$, "insufficient credit"); yyerror("insufficient credit"); /* note the difference in behavior*/ }
 | idle S			{ printf("credit: 0\n"); }
 ;
a: A    			{ printf("reached 100\n"); }
 | b B  			{ printf("reached 100\n"); }
 | f C  			{ printf("reached 100\n"); }
 | a error			{ lyyerror(@$, "invalid sum: last coin returned"); }
 | a S				{ printf("credit: 100\n"); }
 ;
b: B    			{ printf("reached  50\n"); }
 | c c  			{ printf("reached  50\n"); }
 | b D  			{ lyyerror(@1, "insufficient credit"); /* wrong length here: there is no $1 */ }
 | b error			{ lyyerror(@$, "invalid sum:  last coin returned"); }
 | b S				{ printf("credit:  50\n"); }
 ;
c: C    			{ printf("reached  25\n"); }
 | c D  			{ yyerror("insufficient credit"); }
 | c error			{ lyyerror(@$, "invalid sum:  last coint returned"); }
 | c S				{ printf("credit:  25\n"); }
 ;
d: D    			{ printf("sold!\n"); }
 ;
e: E    			{ lyyerror(@$, "nothing to cancel"); }
 | a E 				{ printf("canceled\n"); }
 | b E 				{ printf("canceled\n"); }
 | c E 				{ printf("canceled\n"); }
 | f E 				{ printf("canceled\n"); }
 ;
f: b C 				{ printf("reached 75\n"); }
 | c B 				{ printf("reached 75\n"); }
 | f D  			{ lyyerror(@$, "insufficient credit"); }
 | f error			{ lyyerror(@$, "invalid sum:  last coin returned"); }
 | f S				{ printf("credit:  75\n"); }
 ;

%%

int main(int argc, char *argv[])
{
	printf("vendfn state machine brutal backporting to BNF\n");
	printf("100 cents for a 'soda', 'cancel' to return money, exact change, accepted '25', '50', '75', '100'\n");
	yydebug = 0;
	yyparse();
}

void yyerror(char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	if(yylloc.first_line)
		printf("file:%s, (l:%d,c:%d)->(l:%d,c:%d)", yylloc.cur_file, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column);
	printf("__yyerror: ");
	vprintf(msg, args);
	printf("\n");
	va_end(args);
	yyclearin;
}

void lyyerror(YYLTYPE t, char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	if(t.first_line)
		printf("file: %s, (l:%d,c:%d)->(l:%d,c:%d)", t.cur_file, t.first_line, t.first_column, t.last_line, t.last_column);
	printf("__lyyerror: ");
	vprintf(msg, args);
	printf("\n");
	va_end(args);
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
