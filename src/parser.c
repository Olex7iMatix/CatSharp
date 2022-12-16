#include "include/parser.h"
#include "include/token.h"
#include "include/types.h"
#include <stdio.h>
#include <string.h>

static scope_T* get_node_scope(parser_T* parser, AST_T* node) {
    return node->scope == (void*) 0 ? parser->scope : node->scope;
}

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->previous_token = parser->current_token;
    parser->scope = init_scope();

    return parser;
}

void parser_eat(parser_T* parser, int token_type, char* str)
{
    if (parser->current_token->type == token_type)
    {
        parser->previous_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        printf(
            "Unexpected token `%s`, with type `%d`. Excepted type `%d`. String to debug: %s\n",
            parser->current_token->value,
            parser->current_token->type,
            token_type,
            str
        );
        exit(1);
    }
}

void parser_skip_token(parser_T* parser)
{
    parser->previous_token = parser->current_token;
    parser->current_token = lexer_get_next_token(parser->lexer);
}

AST_T* parser_parse(parser_T* parser, scope_T* scope)
{
    return parser_parse_statements(parser, scope);
}

AST_T* parser_parse_statement(parser_T* parser, scope_T* scope)
{
    switch (parser->current_token->type)
    {
        case TOKEN_COMMENT: return parser_parse_comment(parser, scope);
        case TOKEN_ID: return parser_parse_id(parser, scope);
        case TOKEN_RETURN: return parser_parse_return(parser, scope);
    }
    
    return init_ast(AST_NOOP);
}

AST_T* parser_parse_statements(parser_T* parser, scope_T* scope)
{
    AST_T* compound = init_ast(AST_COMPOUND);
    compound->scope = scope;
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* ast_statement = parser_parse_statement(parser, scope);
    ast_statement->scope = scope;
    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;

    while (parser->current_token->type == TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_SEMI, "statements");

        AST_T* ast_statement = parser_parse_statement(parser, scope);
        if (ast_statement) {
            compound->compound_size += 1;
            compound->compound_value = realloc(
                compound->compound_value,
                compound->compound_size * sizeof(struct AST_STRUCT*)
            );
            compound->compound_value[compound->compound_size-1] = ast_statement;
        }
    }

    return compound;
}

AST_T* parser_parse_expr(parser_T* parser, scope_T* scope)
{
    switch (parser->current_token->type)
    {
        case TOKEN_STRING: return parser_parse_string(parser, scope);
        case TOKEN_INT: return parser_parse_int(parser, scope);
        case TOKEN_ID: return parser_parse_id(parser, scope);
        case TOKEN_TRUE: return parser_parse_true(parser, scope);
        case TOKEN_FALSE: return parser_parse_false(parser, scope);
    }

    return init_ast(AST_NOOP);
}

char parser_peek(parser_T* parser, int offset) {
    return parser->lexer->content[parser->lexer->i + offset];
}

AST_T* parser_parse_while_expr(parser_T* parser, scope_T* scope) {
    switch (parser->current_token->type)
    {
    case TOKEN_TRUE:
        parser_eat(parser, TOKEN_TRUE, "UwU");
        return init_ast(AST_TRUE);
    case TOKEN_FALSE:
        parser_eat(parser, TOKEN_FALSE, "UwU");
        return init_ast(AST_FALSE);
    // case TOKEN_TRUE:
    //     parser_eat(parser, TOKEN_TRUE, "UwU");
    //     return init_ast(AST_TRUE);
    default:
        return parser_parse_op_expr(parser, scope);
    }
}

AST_T* parser_parse_op_expr(parser_T* parser, scope_T* scope)
{
    if (parser->current_token->type == TOKEN_ID) {
        AST_T* vast;
        switch (parser->current_token->type)
        {
        case TOKEN_ID:
            vast = parser_parse_id(parser, scope);
            break;
        case TOKEN_STRING:
            vast = parser_parse_string(parser, scope);
            break;
        case TOKEN_INT:
            vast = parser_parse_int(parser, scope);
            break;
        default:
            printf("[Parser]: Unexpected token `%s`.\n", parser->current_token->value);
            break;
        }
        AST_T* ast = init_ast(AST_OPERATION);
        ast->operation_var = vast;
        switch (parser->current_token->type) {
            case TOKEN_LT:
                ast->operation_operator = parser->current_token; parser_eat(parser, TOKEN_LT, "parser_parse_op_expr"); break;
            case TOKEN_GT:
                ast->operation_operator = parser->current_token; parser_eat(parser, TOKEN_GT, "parser_parse_op_expr"); break;
            case TOKEN_EQUALS:
                parser_eat(parser, TOKEN_EQUALS, "uwu");
                token_T* tok = parser->current_token;
                if (tok->type == TOKEN_EQUALS) {
                    ast->operation_operator = init_token(TOKEN_EQUALS, "==");
                    parser_eat(parser, TOKEN_EQUALS, "parser_parse_op_expr");
                } else {
                    printf("[Parser]: Expected '==' but found '=%s'\n", tok->value);
                    exit(1);
                }
                break;
        }
        AST_T* svast = (void*) 0;
        switch (parser->current_token->type)
        {
        case TOKEN_ID:
            svast = parser_parse_id(parser, scope);
            break;
        case TOKEN_STRING:
            svast = parser_parse_string(parser, scope);
            break;
        case TOKEN_INT:
            svast = parser_parse_int(parser, scope);
            break;
        default:
            printf("[Parser]: Unexpected token `%s`.\n", parser->current_token->value);
            break;
        }
        ast->operation_second_var = svast;

        return ast;
    } else if (parser->current_token->type == TOKEN_TRUE) {
        return init_ast(TOKEN_TRUE);
    } else if (parser->current_token->type == TOKEN_FALSE) {
        return init_ast(TOKEN_FALSE);
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_factor(parser_T* parser, scope_T* scope)
{
}

AST_T* parser_parse_term(parser_T* parser, scope_T* scope)
{
}

AST_T* parser_parse_function_call(parser_T* parser, scope_T* scope)
{
    AST_T* function_call = init_ast(AST_FUNCTION_CALL);

    function_call->function_call_name = parser->previous_token->value;
    
    parser_eat(parser, TOKEN_LPAREN, "fcall LParen");

    if (parser->current_token->type != TOKEN_RPAREN) {
        function_call->function_call_arguments = calloc(1, sizeof(struct AST_STRUCT*));

        AST_T* ast_expr = parser_parse_expr(parser, scope);

        function_call->function_call_arguments[0] = ast_expr;
        function_call->function_call_arguments_size += 1;

        while (parser->current_token->type == TOKEN_COMMA)
        {
            parser_eat(parser, TOKEN_COMMA, "fcall comma");

            AST_T* ast_expr = parser_parse_expr(parser, scope);
            function_call->function_call_arguments_size += 1;
            function_call->function_call_arguments = realloc(
                function_call->function_call_arguments,
                function_call->function_call_arguments_size * sizeof(struct AST_STRUCT*)
            );
            function_call->function_call_arguments[function_call->function_call_arguments_size-1] = ast_expr;
        }
    }
    else {
        function_call->function_call_arguments = (void*)0;
        function_call->function_call_arguments_size = 0;
    }

    parser_eat(parser, TOKEN_RPAREN, "fcall RParen");

    function_call->scope = scope;

    return function_call;
}

AST_T* parser_parse_variable_definition(parser_T* parser, scope_T* scope)
{
    parser_eat(parser, TOKEN_ID, "vardef ID"); // varbuiltin_function_log
    char* variable_definition_variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID, "vardef ID"); // var name
    parser_eat(parser, TOKEN_EQUALS, "vardef equals");

    if (parser->current_token->type == TOKEN_LPAREN) {
        AST_T* ast = init_ast(AST_VARIABLE_FUNCTION_DEFINITION);
        parser_eat(parser, TOKEN_LPAREN, "");

        if (parser->current_token->type != TOKEN_RPAREN) {
            ast->var_function_definition_args = calloc(1, sizeof(struct AST_STRUCT*));

            AST_T* arg = parser_parse_variable(parser, scope);
            ast->var_function_definition_args_size += 1;
            ast->var_function_definition_args[ast->var_function_definition_args_size-1] = arg;

            while (parser->current_token->type == TOKEN_COMMA) {
                parser_eat(parser, TOKEN_COMMA, "fdef");
                ast->var_function_definition_args_size += 1;
                ast->var_function_definition_args = realloc(ast->var_function_definition_args, ast->var_function_definition_args_size * sizeof(struct AST_STRUCT*));

                AST_T* arg = parser_parse_variable(parser, scope);
                ast->var_function_definition_args[ast->var_function_definition_args_size-1] = arg;
            }   
        } else {
            ast->var_function_definition_args = (void*) 0;
            
            ast->function_call_arguments_size = 0;
        }

        parser_eat(parser, TOKEN_RPAREN, "");

        parser_eat(parser, TOKEN_LBRACE, "");

        ast->var_function_definition_body = parser_parse_statements(parser, scope);
        ast->scope = scope;

        parser_eat(parser, TOKEN_RBRACE, "");

        return ast;
    } else {
        AST_T* variable_definition_value = parser_parse_expr(parser, scope);

        AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION);
        variable_definition->variable_definition_variable_name = variable_definition_variable_name;
        variable_definition->variable_definition_value = variable_definition_value;
        
        variable_definition->scope = scope;

        return variable_definition;
    }
}

AST_T* parser_parse_function_definition(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_FUNCTION_DEFINITION);
    parser_eat(parser, TOKEN_ID, "fdef id"); // void
    char* function_name = parser->current_token->value;
    ast->function_definition_name = calloc(strlen(function_name) + 1, sizeof(char));
    strcpy(ast->function_definition_name, function_name);
    parser_eat(parser, TOKEN_ID, "fdef ID"); // void name
    parser_eat(parser, TOKEN_LPAREN, "fdef LPAREN");

    if (parser->current_token->type != TOKEN_RPAREN) {
        ast->function_definition_args = calloc(1, sizeof(struct AST_STRUCT*));

        AST_T* arg = parser_parse_variable(parser, scope);
        ast->function_definition_args_size += 1;
        ast->function_definition_args[ast->function_definition_args_size-1] = arg;

        while (parser->current_token->type == TOKEN_COMMA) {
            parser_eat(parser, TOKEN_COMMA, "fdef");
            ast->function_definition_args_size += 1;
            ast->function_definition_args = realloc(ast->function_definition_args, ast->function_definition_args_size * sizeof(struct AST_STRUCT*));

            AST_T* arg = parser_parse_variable(parser, scope);
            ast->function_definition_args[ast->function_definition_args_size-1] = arg;
        }   
    } else {
        ast->function_definition_args = (void*) 0;
        
        ast->function_call_arguments_size = 0;
    }

    parser_eat(parser, TOKEN_RPAREN, "fdef RParen");
    parser_eat(parser, TOKEN_LBRACE, "fdef LBRACE");

    ast->function_definition_body = parser_parse_statements(parser, scope);
    ast->scope = scope;

    parser_eat(parser, TOKEN_RBRACE, "fdef RBRACE");

    return ast;
}

AST_T* parser_parse_pack_definition(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_PACK_DEFINITION);
    parser_eat(parser, TOKEN_ID, "packdef"); // pack
    char* pack_name = parser->current_token->value;
    ast->pack_definition_name = calloc(strlen(pack_name) + 1, sizeof(char));
    strcpy(ast->pack_definition_name, pack_name);
    parser_eat(parser, TOKEN_ID, "packdef"); // pack name
    parser_eat(parser, TOKEN_LBRACE, "packdef");

    ast->pack_definition_body = parser_parse_statements(parser, scope);
    ast->scope = scope;

    parser_eat(parser, TOKEN_RBRACE, "packdef");

    return ast;
}

AST_T* parser_parse_import_statement(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_IMPORT_STATEMENT);
    parser_eat(parser, TOKEN_ID, "imp");
    char* pack_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID, "imp");
    parser_eat(parser, TOKEN_DOT, "imp");
    char* class_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID, "imp");
    ast->import_statement_imp_name = calloc(strlen(pack_name) + 1, sizeof(char));
    ast->import_statement_class_name = calloc(strlen(class_name) + 1, sizeof(char));
    strcpy(ast->import_statement_imp_name, pack_name);
    strcpy(ast->import_statement_class_name, class_name);
    return ast;
}

AST_T* parser_parse_if_statement(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_IF_STATEMENT);
    parser_eat(parser, TOKEN_ID, "if"); // if
    parser_eat(parser, TOKEN_LPAREN, "if");
    AST_T* ast_expr = parser_parse_op_expr(parser, scope);
    ast->if_arg = ast_expr;
    parser_eat(parser, TOKEN_RPAREN, "if");
    parser_eat(parser, TOKEN_LBRACE, "if");

    ast->if_body = parser_parse_statements(parser, scope);
    ast->scope = scope;

    parser_eat(parser, TOKEN_RBRACE, "if");

    if (parser->current_token->type == TOKEN_ID) {
        if (strcmp (parser->current_token->value, "else") == 0) {
            parser_parse_else_statement(parser, scope, ast);
            return ast;
        }
    }

    return ast;
}

AST_T* parser_parse_while_statement(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_WHILE_STATEMENT);
    parser_eat(parser, TOKEN_ID, "if"); // if
    parser_eat(parser, TOKEN_LPAREN, "if");
    AST_T* ast_expr = parser_parse_while_expr(parser, scope);
    ast->while_arg = ast_expr;
    parser_eat(parser, TOKEN_RPAREN, "if");
    parser_eat(parser, TOKEN_LBRACE, "if");

    ast->while_body = parser_parse_statements(parser, scope);
    ast->scope = scope;

    parser_eat(parser, TOKEN_RBRACE, "if");

    if (parser->current_token->type == TOKEN_ID) {
        if (strcmp (parser->current_token->value, "else") == 0) {
            parser_parse_else_statement(parser, scope, ast);
            return ast;
        }
    }

    return ast;
}

AST_T* parser_parse_else_statement(parser_T* parser, scope_T* scope, AST_T* if_ast)
{
    AST_T* ast = init_ast(AST_ELSE_STATEMENT);
    parser_eat(parser, TOKEN_ID, "else"); // else

    if (parser->current_token->type == TOKEN_LBRACE) {
        parser_eat(parser, TOKEN_LBRACE, "else");

        ast->else_body = parser_parse_statements(parser, scope);
        ast->scope = scope;

        parser_eat(parser, TOKEN_RBRACE, "else");
    } else if (parser->current_token->type == TOKEN_ID) {
        parser_parse_if_statement(parser, scope);
    } else {
        printf("%s\n", parser->current_token->value);
    }

    if_ast->if_else = ast;

    return ast;
}

AST_T* parser_parse_class_definition(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_CLASS_DEFINITION);
    parser_eat(parser, TOKEN_ID, "class"); // class
    char* class_name = parser->current_token->value;
    ast->class_definition_name = calloc(strlen(class_name) + 1, sizeof(char));
    strcpy(ast->class_definition_name, class_name);
    parser_eat(parser, TOKEN_ID, "class"); // class name
    parser_eat(parser, TOKEN_LBRACE, "class");

    ast->class_definition_body = parser_parse_statements(parser, scope);
    ast->scope = scope;

    parser_eat(parser, TOKEN_RBRACE, "class");

    return ast;
}

AST_T* parser_parse_variable(parser_T* parser, scope_T* scope)
{
    char* token_value = parser->current_token->value;
    parser_eat(parser, TOKEN_ID, "var"); // var name or function call name

    if (parser->current_token->type == TOKEN_PLUS) {
        AST_T* previous_var = init_ast(AST_VARIABLE);
        previous_var->variable_name = token_value;
        previous_var->scope = scope;

        parser_eat(parser, TOKEN_PLUS, "plus");

        token_value = parser->current_token->value;
        
        parser_eat(parser, TOKEN_ID, "lmao");

        AST_T* current_var = init_ast(AST_VARIABLE);
        current_var->variable_name = token_value;
        current_var->scope = scope;
        
        return parser_parse_plus(parser, scope, previous_var, current_var);
    }

    if (parser->current_token->type == TOKEN_LPAREN)
        return parser_parse_function_call(parser, scope);

    AST_T* ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_value;
    ast_variable->scope = scope;

    return ast_variable;
}

AST_T* parser_parse_plus(parser_T* parser, scope_T* scope, AST_T* a_1, AST_T* a_2) {
    char* prev_id_name = a_1->variable_name;
    char* cur_id_name = a_2->variable_name;
    AST_T* prev_var = scope_get_var_def(scope, prev_id_name);
    AST_T* cur_var = scope_get_var_def(scope, cur_id_name);
    
    if (!prev_var) {
        printf("[Parser]: Cannot find variable `%s`.", prev_id_name);
        exit(1);
    }
    if (!cur_var) {
        printf("[Parser]: Cannot find variable `%s`.", cur_id_name);
        exit(1);
    }

    AST_T* ast = init_ast(AST_ADD);
    ast->add_prev_var  =   prev_var;
    ast->add_cur_var   =   cur_var;

    return ast;
}

AST_T* parser_parse_string(parser_T* parser, scope_T* scope)
{
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING, "str");

    ast_string->scope = scope;

    return ast_string;
}

AST_T* parser_parse_int(parser_T* parser, scope_T* scope)
{
    AST_T* ast_int = init_ast(AST_INT);
    ast_int->int_value = atoi(parser->current_token->value);
    parser_eat(parser, TOKEN_INT, "int");

    ast_int->scope = scope;

    return ast_int;
}

AST_T* parser_parse_break_keyword(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_BREAK);
    parser_eat(parser, TOKEN_BREAK, "break");
    return ast;
}

AST_T* parser_parse_return_keyword(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_RETURN);
    parser_eat(parser, TOKEN_ID, "return");
    AST_T* val = parser_parse_expr(parser, scope);
    ast->return_value = val;
    return ast;
}

AST_T* parser_parse_id(parser_T* parser, scope_T* scope)
{
    printf("%s\n", parser->current_token->value);
    if (strcmp(parser->current_token->value, "var") == 0)
    {
        return parser_parse_variable_definition(parser, scope);
    }
    else if (strcmp(parser->current_token->value, "void") == 0)
    {
        return parser_parse_function_definition(parser, scope);
    }
    else if (strcmp(parser->current_token->value, "pack") == 0)
    {
        return parser_parse_pack_definition(parser, scope);
    }
    else if (strcmp(parser->current_token->value, "class") == 0)
    {
        return parser_parse_class_definition(parser, scope);
    }
    else if (strcmp(parser->current_token->value, "imp") == 0)
    {
        return parser_parse_import_statement(parser, scope);
    }
    else if (strcmp(parser->current_token->value, "if") == 0)
    {
        return parser_parse_if_statement(parser, scope);
    }
    else if (strcmp(parser->current_token->value, "while") == 0)
    {
        return parser_parse_while_statement(parser, scope);
    }
    else if (strcmp(parser->current_token->value, "return") == 0)
    {
        return parser_parse_return_keyword(parser, scope);
    }
    else if (strcmp(parser->current_token->value, "break") == 0)
    {
        return parser_parse_break_keyword(parser, scope);
    }
    else
    {
        return parser_parse_variable(parser, scope);
    }
}

AST_T* parser_parse_comment(parser_T* parser, scope_T* scope) {
    parser_eat(parser, TOKEN_COMMENT, "comment");
    while (parser->current_token->type != TOKEN_SEMI) {
        parser_skip_token(parser);
    }
    return init_ast(AST_NOOP);
}

AST_T* parser_parse_return(parser_T* parser, scope_T* scope) {
    parser_eat(parser, TOKEN_RETURN, "return");
    AST_T* ast = init_ast(AST_RETURN);
    switch (parser->current_token->type)
    {
    case TOKEN_STRING:
        ast->return_value = init_ast(AST_STRING);
        ast->return_value->string_value = parser->current_token->value;
        parser_eat(parser, TOKEN_STRING, "");
        break;
    default:
        printf("[Parser]: Unsupported return token type `%d`\n", parser->current_token->type);
        exit(1);
    }
    return ast;
}

AST_T* parser_parse_true(parser_T* parser, scope_T* scope) {
    AST_T* ast = init_ast(AST_TRUE);
    parser_eat(parser, TOKEN_TRUE, "true");
    return ast;
}

AST_T* parser_parse_false(parser_T* parser, scope_T* scope) {
    AST_T* ast = init_ast(AST_FALSE);
    parser_eat(parser, TOKEN_FALSE, "false");
    return ast;
}