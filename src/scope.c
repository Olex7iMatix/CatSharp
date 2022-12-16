#include "include/scope.h"
#include <string.h>

scope_T* init_scope() {
    scope_T* scope = calloc(1, sizeof(struct SCOPE_STRUCT));
    scope->function_definitions = (void*) 0;
    scope->function_definitions_size = 0;
    scope->variable_definitions = (void*) 0;
    scope->variable_definitions_size = 0;
    scope->var_function_definitions = (void*) 0;
    scope->var_function_definitions_size = 0;
    scope->additional_scopes = (void*) 0;
    scope->scope_name = (void*) 0;

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

    for (int i = 0; i < scope->additional_scopes_size; i++) {
        scope_T* _scope = scope->additional_scopes[i];
        
        AST_T* ast = scope_get_function_def(_scope, fname);

        if (ast != (void*) 0) return ast;
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

    for (int i = 0; i < scope->additional_scopes_size; i++) {
        scope_T* _scope = scope->additional_scopes[i];

        AST_T* ast = scope_get_var_def(_scope, name);

        if (ast != (void*) 0) return ast; 
    }

    return (void*) 0;
}

AST_T* scope_add_var_function_def(scope_T* scope, AST_T* vdef) {
    if (scope->var_function_definitions == (void*) 0) {
        scope->var_function_definitions = calloc(1, sizeof(struct AST_STRUCT));
        scope->var_function_definitions[0] = vdef;
        scope->var_function_definitions_size += 1;
    } else {
        scope->var_function_definitions_size += 1;
        scope->var_function_definitions = realloc(
            scope->var_function_definitions,
            scope->var_function_definitions_size * sizeof(struct AST_STRUCT*)
        );
        scope->var_function_definitions[scope->var_function_definitions_size-1] = vdef;
    }

    return vdef;
}

AST_T* scope_get_var_function_def(scope_T* scope, const char* name) {
    for (int i = 0; i < scope->var_function_definitions_size; i++) {
        AST_T* def = scope->var_function_definitions[i];

        if (strcmp(def->var_function_definition_name, name) == 0) return def;
    }

    for (int i = 0; i < scope->additional_scopes_size; i++) {
        scope_T* _scope = scope->additional_scopes[i];

        AST_T* ast = scope_get_var_function_def(_scope, name);

        if (ast != (void*) 0) return ast;
    }

    return (void*) 0;
}

void add_scope_to_scope(scope_T* scope, scope_T* new_scope, char* class_name) {
    if (scope->additional_scopes == (void*) 0) {
        scope->additional_scopes = calloc(1, sizeof(struct SCOPE_STRUCT));
        scope->additional_scopes[0] = new_scope;
        scope->additional_scopes_size += 1;
    } else {
        scope->additional_scopes_size += 1;
        scope->additional_scopes = realloc(
            scope->additional_scopes,
            scope->additional_scopes_size * sizeof(struct SCOPE_STRUCT*));
        new_scope->scope_name = class_name;
        scope->additional_scopes[scope->additional_scopes_size-1] = new_scope;
    }
}

int check_classname_in_scope(scope_T* scope, char* class_name) {
    for (int i = 0; i < scope->additional_scopes_size; i++) {
        scope_T* _scope = scope->additional_scopes[i];

        if (_scope->scope_name == class_name) return 1;
    }

    return 0;
}