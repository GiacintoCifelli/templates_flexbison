all: testast testhash teststack testdllist

testast: testast.c ast.c ast.h
	gcc -g -save-temps -o testast ast.c testast.c

testhash: testhash.c hash.c hash.h
	gcc -g -save-temps -o testhash hash.c testhash.c

teststack: teststack.c stack.c stack.h
	gcc -g -save-temps -o teststack stack.c teststack.c

testdllist: testdllist.c dllist.c dllist.h
	gcc -g -save-temps -o testdllist dllist.c hash.c testdllist.c

check: all
	strace ./testast
	strace ./testhash
	strace ./teststack
	strace ./testdllist
	valgrind --leak-check=full ./testast
	valgrind --leak-check=full ./testhash
	valgrind --leak-check=full ./teststack
	valgrind --leak-check=full ./testdllist
	./testast
	./testhash
	./teststack
	./testdllist

clean:
	touch testast testhash teststack testdllist core a.i a.o a.s
	rm testast testhash teststack testdllist core *.i *.o *.s

help:
	@grep '^[^#[:space:]].*:' Makefile | sort

.PHONY: help check
