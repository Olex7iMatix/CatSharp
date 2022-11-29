#include "include/scope.h"
#include <string.h>

scope_T* init_scope() {
    scope_T* scope = calloc(1, sizeof(struct SCOPE_STRUCT));
    scope->function_definitions = (void*) 0;
    scope->function_definitions_size = 0;
    scope->variable_definitions = (void*) 0;
    scope->variable_definitions_size = 0;

    return scope;
}

AST_T* scope_add_function_def(scope_T* scope, AST_T* fdef) {
    scope->function_definitions_size += 1;

    if (scope->function_definitions == (void*) 0) {
        scope->function_definitions = calloc(1, sizeof(struct AST_STRUCT*));
    } else {
        scope->function_definitions = realloc(scope->function_definitions, scope->function_definitions_size * sizeof(struct AST_STRUCT**));
    }

    scope->function_definitions[scope->function_definitions_size-1] = fdef;

    return fdef;
}

AST_T* scope_get_function_def(scope_T* scope, const char* fname) {
    for (int i = 0; i < scope->function_definitions_size; i++) {
        AST_T* def = scope->function_definitions[i];

        if (strcmp(def->function_definition_name, fname) == 0) return def;
    }

    return (void*) 0;
}

AST_T* scope_add_var_def(scope_T* scope, AST_T* vdef) {
    if (scope->variable_definitions == (void*) 0) {
        scope->variable_definitions = calloc(1, sizeof(struct AST_STRUCT));
        scope->variable_definitions[0] = vdef;
        scope->variable_definitions_size += 1;
    } else {
        scope->variable_definitions_size += 1;
        scope->variable_definitions = realloc(
            scope->variable_definitions,
            scope->variable_definitions_size * sizeof(struct AST_STRUCT*)
        );
        scope->variable_definitions[scope->variable_definitions_size-1] = vdef;
    }

    return vdef;
}

AST_T* scope_get_var_def(scope_T* scope, const char* name) {
    for (int i = 0; i < scope->variable_definitions_size; i++) {
        AST_T* def = scope->variable_definitions[i];

        if (strcmp(def->variable_definition_variable_name, name) == 0) return def;
    }

    return (void*) 0;
}