all: testast testhash teststack

testast: testast.c ast.c ast.h
	gcc -g -o testast ast.c testast.c

testhash: testhash.c hash.c hash.h
	gcc -g -o testhash hash.c testhash.c

teststack: teststack.c stack.c stack.h
	gcc -g -o teststack stack.c teststack.c

check: all
	valgrind --leak-check=full ./testast
	valgrind --leak-check=full ./testhash
	valgrind --leak-check=full ./teststack
	strace ./testast
	strace ./testhash
	strace ./teststack
	./testast
	./testhash
	./teststack

clean:
	touch testast testhash teststack core
	rm testast testhash teststack core

help:
	@grep '^[^#[:space:]].*:' Makefile | sort

.PHONY: help check
