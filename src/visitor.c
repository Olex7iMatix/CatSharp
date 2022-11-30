#include "include/visitor.h"
#include "include/AST.h"
#include "include/parser.h"
#include "include/lexer.h"
#include "include/io.h"
#include "include/scope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static AST_T* builtin_function_log(visitor_T* visitor, AST_T** args, int args_size) {
    for (int i = 0; i < args_size; i++) {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
        case AST_STRING:
            printf("%s\n", visited_ast->string_value);
            break;
        default:
            printf("%p\n", visited_ast);
            break;
        }
    }

    return init_ast(AST_NOOP);
}

visitor_T* init_visitor() {
    visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));

    return visitor;
}

AST_T* visitor_visit(visitor_T* visitor, AST_T* node) {
    switch (node->type)
    {
        case AST_VARIABLE_DEFINITION: return visitor_visit_variable_definition(visitor, node); break;
        case AST_VARIABLE: return visitor_visit_variable(visitor, node); break;
        case AST_FUNCTION_CALL: return visitor_visit_function_call(visitor, node); break;
        case AST_FUNCTION_DEFINITION: return visitor_visit_function_definition(visitor, node); break;
        case AST_PACK_DEFINITION: return visitor_visit_pack(visitor, node); break;
        case AST_CLASS_DEFINITION: return visitor_visit_class(visitor, node); break;
        case AST_IMPORT_STATEMENT: return visitor_visit_import_statement(visitor, node); break;
        case AST_IF_STATEMENT: return visitor_visit_if_statement(visitor, node); break;
        case AST_ELSE_STATEMENT: return visitor_visit_else_statement(visitor, node); break;
        case AST_STRING: return visitor_visit_string(visitor, node); break;
        case AST_COMPOUND: return visitor_visit_compound(visitor, node); break;
        case AST_NOOP: return node; break;
    }

    printf("Uncaught statement of type `%d`\n", node->type);
    exit(1);

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_if_statement(visitor_T* visitor, AST_T* node) {
    if (strcmp(node->if_arg->variable_name, "true") == 0) {
        visitor_visit(visitor, node->if_body);
    } else if (strcmp(node->if_arg->variable_name, "false") == 0) {
        visitor_visit(visitor, node->if_else->else_body);
    }
    return node;
}

AST_T* visitor_visit_else_statement(visitor_T* visitor, AST_T* node) {

}

AST_T* visitor_visit_import_statement(visitor_T* visitor, AST_T* node) {
    char* name = node->import_statement_imp_name;

    lexer_T* lexer = init_lexer(get_file_content(name));

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser, node->scope);
    visitor_T* ast_visitor = init_visitor();
    visitor_visit(ast_visitor, root);

    return node;
}

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node) {
    scope_add_var_def(node->scope, node);

    return node;
}

AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node) {
    scope_add_function_def(
        node->scope,
        node
    );
    
    return node;
}

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node) {
    AST_T* vdef = scope_get_var_def(node->scope, node->variable_name);
    
    if (vdef != (void*) 0)
        return visitor_visit(visitor, vdef->variable_definition_value);

    printf("Undefined variable `%s`\n", node->variable_name);
    exit(1);
}

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node) {
    if (strcmp(node->function_call_name, "log") == 0)
    {
        return builtin_function_log(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }

    AST_T* fdef = scope_get_function_def(node->scope, node->function_call_name);

    if (fdef == (void*) 0){
        printf("Undefined method `%s`\n", node->function_call_name);
        exit (1);
    }

    for (int i = 0; i < node->function_call_arguments_size; i++) {
        AST_T* ast_var = (AST_T*) fdef->function_definition_args[i];
        AST_T* ast_value = (AST_T*) node->function_call_arguments[i];

        AST_T* var_def = init_ast(AST_VARIABLE_DEFINITION);
        var_def->variable_definition_variable_name = calloc(strlen(ast_var->variable_name)+ 1, sizeof(char));
        strcpy(var_def->variable_definition_variable_name, ast_var->variable_name);
        var_def->variable_definition_value = ast_value;
        
        scope_add_var_def(fdef->function_definition_body->scope, var_def);
    }
    
    return visitor_visit(visitor, fdef->function_definition_body);
}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node) {
    return node;
}

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node) {
    for (int i = 0; i < node->compound_size; i++) {
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_pack(visitor_T* visitor, AST_T* node) {
    for (int i = 0; i < node->pack_definition_body->compound_size; i++) {
        visitor_visit(visitor, node->pack_definition_body->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_class(visitor_T* visitor, AST_T* node) {
    for (int i = 0; i < node->class_definition_body->compound_size; i++) {
        visitor_visit(visitor, node->class_definition_body->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}