/**********************************************************************************************
build with:
	gcc -g -o testast ast.c testast.c

run with:
	valgrind --leak-check=full ./testast

expected program output when run with: ./testast:
int: 5
string: 'aooo'

**********************************************************************************************/

#include <stdio.h>
#include <string.h>

#include "ast.h"

val_t parse_my_ast(ast_t *a, void *user_data) {
	val_t res;
	memset(&res, 0, sizeof(res));
	if(!a) return res;
	switch(a->type) {
	case AST_TYPE_CHILD:
		parse_my_ast(a->child, user_data);
		break;
	case AST_TYPE_LR:
		parse_my_ast(a->left, user_data);
		parse_my_ast(a->right, user_data);
		break;
	case AST_TYPE_BRANCHES:
	case AST_TYPE_BRANCHES_DYNAMIC:
		for(int i=0;i<a->nb_branches;i++)
			parse_my_ast(a->branches[i], user_data); /* only last res kept in this example */
		break;
	case AST_TYPE_INT:
		printf("int: %d\n", a->i);
		res.i = a->i;
		break;
	case AST_TYPE_DOUBLE:
		printf("double: %f\n", a->d);
		res.d = a->d;
		break;
	case AST_TYPE_STRING:
		printf("string: '%s'\n", a->s);
		res.s = a->s;
		break;
	case AST_TYPE_VOID:
		printf("pointer: %p\n", a->v);
		res.v = a->v;
		break;
	}
	return res;
}

int main(void) {
	ast_t *astl = new_ast(AST_TYPE_INT, 5);
	ast_t *astr = new_ast(AST_TYPE_STRING, "aooo");
	ast_t *ast2 = new_ast(AST_TYPE_LR, 0, astl, astr);
	ast_t *ast1 = new_ast(AST_TYPE_CHILD, 0, ast2);
	ast_t *ast = new_ast(AST_TYPE_BRANCHES, 0, 1, ast1);
	parse_my_ast(ast, NULL);
	free_ast(ast);
	return 0;
}
