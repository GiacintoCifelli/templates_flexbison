all:
	flex basic.l
	bison basic.y
	gcc -g -o basic lexer.c parser.c

check:
	cd tests; ../basic < test.c

clean:
	rm parser.c lexer.c basic
