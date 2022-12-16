#ifndef CATS_VISITOR
#define CATS_VISITOR
#include "AST.h"

typedef struct VISITOR_STRUCT {
} visitor_T;

visitor_T* init_visitor();

AST_T* visitor_visit(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_operation(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_import_statement(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_if_statement(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_while_statement(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_else_statement(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_while_statement(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_true(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_false(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_break(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_return(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_add(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_var_function_definition(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_var_function_call(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_pack(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

AST_T* visitor_visit_class(visitor_T* visitor, AST_T* node, struct SCOPE_STRUCT* scope);

#endif