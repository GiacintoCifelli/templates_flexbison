/****************************************************************************************************

generic abstract syntax tree structure and functions

****************************************************************************************************/

#ifndef _AST_H_
#define _AST_H_

#include <stdbool.h>

typedef enum {
	AST_TYPE_INT			= 1,
	AST_TYPE_DOUBLE			= 2,
	AST_TYPE_STRING			= 3,
	AST_TYPE_CHILD			= 4,
	AST_TYPE_LR			= 5,
	AST_TYPE_BRANCHES		= 6,
	AST_TYPE_BRANCHES_DYNAMIC	= 60,
	AST_TYPE_VOID			= 7,
} ast_type_t;

typedef union {
	int i;
	double d;
	char *s;
	void *v;
} val_t;

typedef struct ast_t ast_t;
struct ast_t {
	ast_type_t type;
	union {
		int i;
		double d;
		char *s;
		struct {
			int operation;
			union {
				ast_t *child;
				struct {
					ast_t *left;
					ast_t *right;
				};
				struct {
					int nb_branches;
					ast_t **branches;
				};
			};
		};
		struct {
			int voidtype;
			void *v;
		};
	};
	ast_t *next;
	ast_t *prev;
};

ast_t *new_ast(ast_type_t type, ...);
void free_ast_extended(ast_t *ast, bool free_vars, void(*freevoid)(int voidtype, void *v));
void free_ast(void *ast);  /* prototype compatible with free(), strings and void* not deallocated */

#ifdef AST_INCLUDE_TEMPLATE
val_t parse_ast_template(ast_t *ast, void(*process_node)(ast_t *ast, void *user_data), void *user_data);
#endif /* AST_INCLUDE_TEMPLATE */

#endif /* _AST_H_ */
