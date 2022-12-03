#ifndef CATS_SCOPE
#define CATS_SCOPE

#include "AST.h"

typedef struct SCOPE_STRUCT {
    AST_T** function_definitions;
    size_t function_definitions_size;
    AST_T** variable_definitions;
    size_t variable_definitions_size;
    struct SCOPE_STRUCT** additional_scopes;
    size_t additional_scopes_size;
    char* scope_name;
} scope_T;

scope_T* init_scope();

AST_T* scope_add_function_def(scope_T* scope, AST_T* fdef);

AST_T* scope_get_function_def(scope_T* scope, const char* fname);

AST_T* scope_add_var_def(scope_T* scope, AST_T* vdef);

AST_T* scope_get_var_def(scope_T* scope, const char* name);

void add_scope_to_scope(scope_T* scope, scope_T* new_scope, char* class_name);

int check_classname_in_scope(scope_T* scope, char* class_name);

#endif