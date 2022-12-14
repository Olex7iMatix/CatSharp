#include "include/visitor.h"
#include "include/AST.h"
#include "include/parser.h"
#include "include/lexer.h"
#include "include/io.h"
#include "include/scope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static AST_T* builtin_function_log(visitor_T* visitor, AST_T** args, int args_size, scope_T* scope) {
    for (int i = 0; i < args_size; i++) {
        AST_T* visited_ast = visitor_visit(visitor, args[i], scope);

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

#include "include/io.h"

static AST_T* builtin_function_writeToFile(visitor_T* visitor, AST_T** args, int args_size, scope_T* scope) {
    if (args_size != 2) {
        printf("[Visitor]: Function `writeToFile` have more/less arguments.");
        exit(1);
    }

    char* filename = args[0]->string_value;
    if (filename == (void*) 0) {
        printf("[Visitor]: Argument 0 in function `writeToFile` have to be a string.");
        exit(1);
    }

    char* content = args[1]->string_value;
    if (content == (void*) 0) {
        printf("[Visitor]: Argument 1 in function `writeToFile` have to be a string.");
        exit(1);
    }

    writeToFile(filename, content);

    return init_ast(AST_NOOP);
}

visitor_T* init_visitor() {
    visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));

    return visitor;
}

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node, scope_T* scope) {
    for (int i = 0; i < node->compound_size; i++) {
        visitor_visit(visitor, node->compound_value[i], scope);
    }

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit(visitor_T* visitor, AST_T* node, scope_T* scope) {
    switch (node->type)
    {
        case AST_VARIABLE_DEFINITION: return visitor_visit_variable_definition(visitor, node, scope); break;
        case AST_VARIABLE: return visitor_visit_variable(visitor, node, scope); break;
        case AST_FUNCTION_CALL: return visitor_visit_function_call(visitor, node, scope); break;
        case AST_OPERATION: return visitor_visit_operation(visitor, node, scope); break;
        case AST_FUNCTION_DEFINITION: return visitor_visit_function_definition(visitor, node, scope); break;
        case AST_VARIABLE_FUNCTION_DEFINITION: return visitor_visit_var_function_definition(visitor, node, scope); break;
        case AST_PACK_DEFINITION: return visitor_visit_pack(visitor, node, scope); break;
        case AST_CLASS_DEFINITION: return visitor_visit_class(visitor, node, scope); break;
        case AST_IMPORT_STATEMENT: return visitor_visit_import_statement(visitor, node, scope); break;
        case AST_IF_STATEMENT: return visitor_visit_if_statement(visitor, node, scope); break;
        case AST_ELSE_STATEMENT: return visitor_visit_else_statement(visitor, node, scope); break;
        case AST_WHILE_STATEMENT: return visitor_visit_while_statement(visitor, node, scope); break;
        case AST_TRUE: return visitor_visit_true(visitor, node, scope); break;
        case AST_FALSE: return visitor_visit_false(visitor, node, scope); break;
        case AST_BREAK: return visitor_visit_break(visitor, node, scope); break;
        case AST_RETURN: return visitor_visit_return(visitor, node, scope); break;
        case AST_ADD: return visitor_visit_add(visitor, node, scope); break;
        case AST_STRING: return visitor_visit_string(visitor, node, scope); break;
        case AST_COMPOUND: return visitor_visit_compound(visitor, node, scope); break;
        case AST_NOOP: return node; break;
    }

    printf("Uncaught statement of type `%d`\n", node->type);
    exit(1);

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_operation(visitor_T* visitor, AST_T* node, scope_T* scope) {
    return node;
}

AST_T* visitor_visit_if_statement(visitor_T* visitor, AST_T* node, scope_T* scope) {
    AST_T* var = (void*) 0;
    char* var_str = (void*) 0;
    AST_T* svar = (void*) 0;
    char* svar_str = (void*) 0;
    switch (node->if_arg->type)
    {
    case 9:
        return visitor_visit(visitor, node->if_body, scope);
        break;
    case 10:
        return visitor_visit(visitor, node->if_else->else_body, scope);
        break;
    case AST_OPERATION:
        switch (node->if_arg->operation_operator->type)
        {
        case TOKEN_EQUALS:
            switch (node->if_arg->operation_var->type)
            {
            case AST_VARIABLE:
                var = scope_get_var_def(node->scope, node->if_arg->operation_var->variable_name);
                if (var == (void*) 0) {
                    printf("[Visitor]: Cannot find variable '%s'\n", node->if_arg->operation_var->variable_name);
                    exit(1);
                }
                break;
            case AST_STRING:
                var_str = node->if_arg->operation_var->string_value;
                break;
            default:
                break;
            }
            switch (node->if_arg->operation_second_var->type)
            {
            case AST_VARIABLE:
                svar = scope_get_var_def(node->scope, node->if_arg->operation_second_var->variable_name);
                if (var == (void*) 0) {
                    printf("[Visitor]: Cannot find variable '%s'\n", node->if_arg->operation_second_var->variable_name);
                    exit(1);
                }
                break;
            case AST_STRING:
                svar_str = node->if_arg->operation_second_var->string_value;
                break;
            default:
                break;
            }
            if (var != (void*) 0) {
                if (svar != (void*) 0) {
                    if (strcmp(var->variable_definition_value->string_value, svar->variable_definition_value->string_value) == 0) {
                        return visitor_visit(visitor, node->if_body, scope);
                    } else {
                        return visitor_visit(visitor, node->if_else->else_body, scope);
                    }
                } else {
                    if (strcmp(var->variable_definition_value->string_value, svar_str) == 0) {
                        return visitor_visit(visitor, node->if_body, scope);
                    } else {
                        return visitor_visit(visitor, node->if_else->else_body, scope);
                    }
                }
            }
            break;
        default:
            printf("[Visitor]: Unexpected operation.");
            exit(1);
        }
        break;
    case 1:
        var = scope_get_var_def(node->scope, node->if_arg->variable_name);

        if (var == (void*) 0) {
            printf("[Error]: Cannot find variable!\n");
            exit(1);
        }

        switch (var->type) {
            case 9:
                return visitor_visit(visitor, node->if_body, scope);
                break;
            case 10:
                return visitor_visit(visitor, node->if_else->else_body, scope);
                break;
            default:
                break;
        }
        break;
    default:
        break;
    }
    return node;
}

AST_T* visitor_visit_else_statement(visitor_T* visitor, AST_T* node, scope_T* scope) {
    return node;
}

AST_T* visitor_visit_while_statement(visitor_T* visitor, AST_T* node, scope_T* scope) {
    AST_T* ast = (void*)0;

    switch (node->while_arg->type)
    {
    case AST_TRUE:
        while (1) {
            if (node->while_isBroken == 0) ast = visitor_visit(visitor, node->while_body, node->scope);
            else break;
        }
        break;
    }

    if (ast == (void*)0) {
        return node;
    }

    return ast;
}

AST_T* visitor_visit_true(visitor_T* visitor, AST_T* node, scope_T* scope) {
    return node;
}

AST_T* visitor_visit_false(visitor_T* visitor, AST_T* node, scope_T* scope) {
    return node;
}

AST_T* visitor_visit_break(visitor_T* visitor, AST_T* node, scope_T* scope) {
    if (node->while_body = (void*) 0) {
        printf("[Visitor]: Cannot use `break` outside while statement.\n");
        exit(1);
    }
    node->while_isBroken = 1;

    return node;
}

AST_T* visitor_visit_return(visitor_T* visitor, AST_T* node, scope_T* scope) {
    AST_T* ast = scope_get_var_function_def(scope, node->var_function_definition_name);

    if (ast == (void*) 0) {
        printf("[Visitor]: Cannot use `return` outside variable function definition.\n");
        exit(1);
    }

    printf("%s\n", node->return_value->string_value);

    AST_T* str = init_ast(AST_STRING);
    str->string_value = node->return_value->string_value;
    ast->var_function_definition_return_variable = str;

    return node;
}

AST_T* visitor_visit_add(visitor_T* visitor, AST_T* node, scope_T* scope) {
    AST_T* ast = init_ast(AST_VARIABLE_DEFINITION);

    AST_T* ast_val = init_ast(AST_STRING);
    char* value;
    sprintf(value, "%s%s", node->add_prev_var->variable_definition_value->string_value, node->add_cur_var->variable_definition_value->string_value);
    ast_val->string_value = value;

    ast->variable_definition_value = ast_val;

    return ast;
}

#include "include/toml.h"

AST_T* visitor_visit_import_statement(visitor_T* visitor, AST_T* node, scope_T* scope) {
    char* name = node->import_statement_imp_name;
    char* class_name = node->import_statement_class_name;

    FILE* fp;
    char errbuf[200];

    fp = fopen("libs.toml", "r");

    if (!fp) {
        printf("[TOML] Error: Could not open file `libs.toml`\n");
        exit(1);
    }

    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));

    if (!conf) {
        printf("[TOML] Error: Cannot parse file `libs.toml`\n");
        exit(1);
    }

    toml_table_t* modules = toml_table_in(conf, "modules");
    if (!modules) {
        printf("[TOML] Error: Missing [modules]!\n");
        exit(1);
    }

    toml_datum_t packPath = toml_string_in(modules, name);
    if (!packPath.ok) {
        printf("[TOML] Error: Cannot read modules.%s\n", name);
        exit(1);
    }

    lexer_T* lexer = init_lexer(get_file_content(packPath.u.s));

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser, node->scope);
    visitor_T* ast_visitor = init_visitor();
    visitor_visit(ast_visitor, root, scope);

    return node;
}

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node, scope_T* scope) {
    scope_add_var_def(scope, node);

    return node;
}

AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node, scope_T* scope) {
    scope_add_function_def(
        scope,
        node
    );
    
    return node;
}

AST_T* visitor_visit_var_function_definition(visitor_T* visitor, AST_T* node, scope_T* scope) {
    AST_T* var = node->var_function_definition_body;
    scope_add_var_function_def(scope, node);
    
    return node;
}

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node, scope_T* scope) {
    printf("%s\n", node->variable_name);
    AST_T* vdef = scope_get_var_def(node->scope, node->variable_name);
    printf("%s\n", vdef->variable_definition_value);
    
    if (vdef != (void*) 0)
        return visitor_visit(visitor, vdef->variable_definition_value, scope);

    printf("Undefined variable `%s`\n", node->variable_name);
    exit(1);
}

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node, scope_T* scope) {
    if (strcmp(node->function_call_name, "log") == 0)
    {
        return builtin_function_log(visitor, node->function_call_arguments, node->function_call_arguments_size, scope);
    } else if (strcmp(node->function_call_name, "writeToFile") == 0)
    {
        return builtin_function_writeToFile(visitor, node->function_call_arguments, node->function_call_arguments_size, scope);
    } else if (strcmp(node->function_call_name, "readFromFile") == 0)
    {
        // node->function_call_arguments, node->function_call_arguments_size, scope
        if (node->function_call_arguments_size != 1) {
            printf("[Visitor]: Function you are trying to call have more/less arguments.");
            exit(1);
        }
        AST_T* return_val = init_ast(AST_STRING);
        return_val->string_value = get_file_content(node->function_call_arguments[0]->string_value);
        return return_val;
    }

    AST_T* fdef = scope_get_function_def(node->scope, node->function_call_name);
    if (fdef == (void*) 0) {
        AST_T* uwu = scope_get_var_function_def(node->scope, node->function_call_name);

        int count = 0;

        for (int i = 0; i < uwu->var_function_definition_args_size; i++) {
            AST_T* ast_var = (AST_T*) uwu->var_function_definition_args[i];
            AST_T* ast_value = (AST_T*) node->function_call_arguments[i];

            AST_T* var_def = init_ast(AST_VARIABLE_DEFINITION);
            var_def->variable_definition_variable_name = calloc(strlen(ast_var->variable_name)+ 1, sizeof(char));
            strcpy(var_def->variable_definition_variable_name, ast_var->variable_name);
            var_def->variable_definition_value = ast_value;
            
            count += 1;

            scope_add_var_def(uwu->var_function_definition_body->scope, var_def);
        }

        if (node->function_call_arguments_size != count) {
            printf("[Visitor]: Function you are trying to call have less or more arguments!\n");
            exit(1);
        }
        
        AST_T* vfdef = visitor_visit(visitor, uwu->var_function_definition_body, scope);

        if (vfdef->var_function_definition_return_variable == (void*)0) {
            printf("[Visitor]: Variable function should have `return` statement.\n");
            exit(1);
        }

        AST_T* return_val = init_ast(AST_STRING);
        return_val->string_value = vfdef->var_function_definition_return_variable->string_value;

        AST_T* return_vdef = init_ast(AST_VARIABLE);
        // return_vdef->variable_definition_value = return_val;
        // return_vdef->variable_name = "msg";
        return_vdef->variable_definition_value = init_ast(AST_STRING);
        return_vdef->variable_definition_value->string_value = "UwU";
        return_vdef->variable_definition_variable_name = "msg";

        AST_T* returned_ast = scope_add_var_def(node->scope, return_vdef);
        printf("var name: %s\n", returned_ast->variable_definition_variable_name);
    } else {
        return visitor_visit(visitor, fdef->function_definition_body, scope);
    }

    printf("Undefined method `%s`\n", node->function_call_name);
    exit(1);
}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node, scope_T* scope) {
    return node;
}

AST_T* visitor_visit_pack(visitor_T* visitor, AST_T* node, scope_T* scope) {
    for (int i = 0; i < node->pack_definition_body->compound_size; i++) {
        visitor_visit(visitor, node->pack_definition_body->compound_value[i], scope);
    }

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_class(visitor_T* visitor, AST_T* node, scope_T* scope) {
    for (int i = 0; i < node->class_definition_body->compound_size; i++) {
        scope_T* new_scope = init_scope();
        visitor_visit(visitor, node->class_definition_body->compound_value[i], new_scope);
        add_scope_to_scope(node->scope, new_scope, node->class_definition_name);
    }

    return init_ast(AST_NOOP);
}