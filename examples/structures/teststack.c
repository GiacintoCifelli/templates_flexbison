/**********************************************************************************************
build with:
	gcc -g -o teststack stack.c teststack.c

run with:
	valgrind --leak-check=full ./teststack

**********************************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "stack.h"

int main(void) {
	stack_t *stack = new_stack();
	stack_push(stack, strdup("test"));
	free(stack_pop(stack));
	free(stack_pop(stack));
	free(stack_pop(stack));
	stack_push(stack, strdup("test"));
	stack_push(stack, strdup("test"));
	free_stack(stack, free);
	stack = new_stack();
	stack_push(stack, "test");
	stack_push(stack, "test");
	stack_push(stack, "test");
	free_stack(stack, NULL);
	return 0;
}
