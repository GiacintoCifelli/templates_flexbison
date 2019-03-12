#include <stdint.h>
#include <stdlib.h>

#include "stack.h"

stack_t *new_stack() {
	return calloc(1, sizeof(stack_t));
}

void free_stack(stack_t *stack, void(*free_record)(void *record)) {
	if(!stack) return;
	void *top;
	do {
		top = stack_pop(stack);
		if(free_record)
			free_record(top);
	} while(top); /* pointer value preserved even if deallocated */
	free(stack);
}

void stack_push(stack_t *stack, void *record) {
	stack_element_t* top = calloc(1, sizeof(stack_element_t));
	top->record = record;
	top->prev = stack->top;
	stack->top = top;
	stack->depth++;
}

void *stack_pop(stack_t *stack) {
	stack_element_t* top = stack->top;
	if(!top) return NULL;
	stack->depth--;
	stack->top = stack->top->prev;
	void *elem = top->record;
	free(top);
	return elem;
}

/**** example of walk-through *********
bool is_in_stack(stack_t *stack, void *symbol) {
	stack_element_t* e = stack->top;
	while(e) {
		if(strcmp(e->record, symbol)==0) return true; // assume elements of type string for comparison
		e = e->prev;
	};
	return false;
}
***************************************/
