#include <stdio.h>

#include "test.h"

/* comment */

//#define I long

//#define I int /* the overload is actually wrong! */

#define P printf

int printf(char *format, void *args);

int main(I argc, char *argv[]) {
	P("%s\n", "hello world!");
}
