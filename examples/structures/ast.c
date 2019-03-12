#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

ast_t *new_ast(ast_type_t type, ...) {
	ast_t *a = calloc(1, sizeof(ast_t));
	va_list args;
	va_start(args, type);
	a->type = type;
	switch(type) {
	case AST_TYPE_INT:
		a->i = va_arg(args, int);
		break;
	case AST_TYPE_DOUBLE:
		a->d = va_arg(args, double);
		break;
	case AST_TYPE_STRING:
		a->s = va_arg(args, char *); /* no strdup here! */
		break;
	case AST_TYPE_CHILD:
		a->operation = va_arg(args, int);
		a->child = va_arg(args, ast_t *);
		break;
	case AST_TYPE_LR:
		a->operation = va_arg(args, int);
		a->left = va_arg(args, ast_t *);
		a->right = va_arg(args, ast_t *);
		break;
	case AST_TYPE_BRANCHES:
		a->operation = va_arg(args, int);
		a->nb_branches = va_arg(args, int);
		a->branches = calloc(a->nb_branches, sizeof(ast_t *));
		for(int i=0;i<a->nb_branches;i++)
			a->branches[i] = va_arg(args, ast_t *);
		break;
	case AST_TYPE_BRANCHES_DYNAMIC:
		a->operation = va_arg(args, int);
		a->nb_branches = va_arg(args, int);
		a->branches = va_arg(args, ast_t **);
		break;
	case AST_TYPE_VOID:
		a->voidtype = va_arg(args, int);
		a->v = va_arg(args, void *);
		break;
	}
	va_end(args);
	return a;
}

void free_ast(void *ast) {
	free_ast_extended(ast, false, NULL);
}

void free_ast_extended(ast_t *a, bool free_vars, void(*freevoid)(int voidtype, void *v)) {
	if(!a) return;
	switch(a->type) {
	case AST_TYPE_INT:
	case AST_TYPE_DOUBLE:
		break;
	case AST_TYPE_STRING:
		if(free_vars) free(a->s);
		break;
	case AST_TYPE_CHILD:
		free_ast_extended(a->child, free_vars, freevoid);
		break;
	case AST_TYPE_LR:
		free_ast_extended(a->left, free_vars, freevoid);
		free_ast_extended(a->right, free_vars, freevoid);
		break;
	case AST_TYPE_BRANCHES:
	case AST_TYPE_BRANCHES_DYNAMIC:
		for(int i=0;i<a->nb_branches;i++)
			free_ast_extended(a->branches[i], free_vars, freevoid);
		free(a->branches);
		break;
	case AST_TYPE_VOID:
		if(free_vars && freevoid) freevoid(a->voidtype, a->v);
		break;
	}
	free(a);
}

#ifdef AST_INCLUDE_TEMPLATE
val_t parse_ast_template(ast_t *a, void *user_data) {
	val_t res;
	memset(&res, 0, sizeof(res));
	if(!a) return res;
	switch(a->type) {
	case AST_TYPE_CHILD:
		res = parse_ast_template(a->child, user_data);
		break;
	case AST_TYPE_LR:
		res = parse_ast_template(a->left, user_data);
		res = parse_ast_template(a->right, user_data); /* only this res kept in this example */
		break;
	case AST_TYPE_BRANCHES:
	case AST_TYPE_BRANCHES_DYNAMIC:
		for(int i=0;i<a->nb_branches;i++)
			res = parse_ast_template(a->branches[i], user_data); /* only last res kept in this example */
		break;
	case AST_TYPE_INT:
		res.i = a->i;
		break;
	case AST_TYPE_DOUBLE:
		res.d = a->d;
		break;
	case AST_TYPE_STRING:
		res.s = a->s;
		break;
	case AST_TYPE_VOID:
		res.v = a->v;
		break;
	}
	return res;
}
#endif /* AST_INCLUDE_TEMPLATE */
