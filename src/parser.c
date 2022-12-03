#include "include/parser.h"
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

AST_T* parser_parse(parser_T* parser, scope_T* scope)
{
    return parser_parse_statements(parser, scope);
}

AST_T* parser_parse_statement(parser_T* parser, scope_T* scope)
{
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser, scope);
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
        case TOKEN_TRUE: return parser_parse_id(parser, scope);
        case TOKEN_FALSE: return parser_parse_id(parser, scope);
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
    parser_eat(parser, TOKEN_RPAREN, "fcall RParen");

    function_call->scope = scope;

    return function_call;
}

AST_T* parser_parse_variable_definition(parser_T* parser, scope_T* scope)
{
    parser_eat(parser, TOKEN_ID, "vardef ID"); // var
    char* variable_definition_variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID, "vardef ID"); // var name
    parser_eat(parser, TOKEN_EQUALS, "vardef equals");
    AST_T* variable_definition_value = parser_parse_expr(parser, scope);

    AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION);
    variable_definition->variable_definition_variable_name = variable_definition_variable_name;
    variable_definition->variable_definition_value = variable_definition_value;
    
    variable_definition->scope = scope;

    return variable_definition;
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
    ast->import_statement_imp_name = calloc(strlen(pack_name) + 1, sizeof(char));
    strcpy(ast->import_statement_imp_name, pack_name);
    return ast;
}

AST_T* parser_parse_if_statement(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_IF_STATEMENT);
    parser_eat(parser, TOKEN_ID, "if"); // if
    parser_eat(parser, TOKEN_LPAREN, "if");
    AST_T* ast_expr = parser_parse_expr(parser, scope);
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

    if (parser->current_token->type == TOKEN_LPAREN)
        return parser_parse_function_call(parser, scope);

    AST_T* ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_value;
    ast_variable->scope = scope;

    return ast_variable;
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

AST_T* parser_parse_class_id(parser_T* parser, scope_T* scope)) {
    AST_T* ast = init_ast(AST_CLASS_ID);
    parser_eat(parser, TOKEN_DOT, "class_id");
}

AST_T* parser_parse_id(parser_T* parser, scope_T* scope)
{

    if (check_classname_in_scope(parser->scope, parser->current_token->value)) return parser_parse_class_id(parser, scope);

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
    else
    {
        return parser_parse_variable(parser, scope);
    }
}