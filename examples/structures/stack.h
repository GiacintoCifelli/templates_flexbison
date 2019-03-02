#ifndef _STACK_H_
#define _STACK_H_

typedef struct stack_element_t stack_element_t;
struct stack_element_t {
	void *record;	/* record stored in the hash map */
	stack_element_t *prev;
};

typedef struct {
	long depth;
	stack_element_t *top;
} stack_t;


stack_t *new_stack();
void free_stack(stack_t *stack, void(*free_elem)(void *));

void stack_push(stack_t *, void *record);
void *stack_pop(stack_t *);

#endif /* _STACK_H_ */
