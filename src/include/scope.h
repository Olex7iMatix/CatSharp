#ifndef CATS_SCOPE
#define CATS_SCOPE

#include "AST.h"

typedef struct SCOPE_STRUCT {
    AST_T** function_definitions;
    size_t function_definitions_size;
} scope_T;

scope_T* init_scope();

AST_T* scope_add_function_def(scope_T* scope, AST_T* fdef);

AST_T* scope_get_function_def(scope_T* scope, const char* fname);

#endif