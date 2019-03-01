all: vending vendfn mpr

vending: vending.l vending.y
	bison vending.y
	flex vending.l
	gcc -g -o vending vending_lexer.c vending_parser.c

vendfn: vendfn.l vendfn.y
	bison vendfn.y
	flex vendfn.l
	gcc -g -o vendfn vendfn_lexer.c vendfn_parser.c

mpr: mpr.l mpr.y
	bison mpr.y
	flex mpr.l
	gcc -g -o mpr mpr_lexer.c mpr_parser.c

clean:
	@touch bogus_parser.c bogus_parser.h bogus_lexer.c bogus_lexer.h vending vendfn mpr bogus.xml bogus.html vending.svg vending_parser.dot
	rm *_parser.[ch] *_lexer.[ch] vending vendfn mpr *.xml *.html *.svg *.dot
