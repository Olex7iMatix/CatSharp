#ifndef CATS_AST
#define CATS_AST
#include <stdlib.h>

typedef struct AST_STRUCT {
    enum {
        AST_VARIABLE_DEFINITION,
        AST_VARIABLE_FUNCTION_DEFINITION,
        AST_VARIABLE,
        AST_FUNCTION_DEFINITION,
        AST_PACK_DEFINITION,
        AST_CLASS_DEFINITION,
        AST_CLASS_ID,
        AST_IMPORT_STATEMENT,
        AST_IF_STATEMENT,
        AST_ELSE_STATEMENT,
        AST_WHILE_STATEMENT,
        AST_BREAK,
        AST_RETURN,
        AST_TRUE,
        AST_FALSE,
        AST_STRING,
        AST_INT,
        AST_FUNCTION_CALL,
        AST_OPERATION,
        AST_NOOP,
        AST_COMPOUND,
    } type;

    struct SCOPE_STRUCT* scope;

    /* AST_VARIABLE_DEFINITION */
    char* variable_definition_variable_name;
    struct AST_STRUCT* variable_definition_value;

    /* AST_VARIABLE */
    char* variable_name;

    /* AST_FUNCTION_CALL */
    char* function_call_name;
    struct AST_STRUCT** function_call_arguments;
    size_t function_call_arguments_size;

    /* AST_OPERATION */
    struct AST_STRUCT* operation_var;
    struct TOKEN_STRUCT* operation_operator;
    struct AST_STRUCT* operation_second_var;
    struct AST_STRUCT* operation_result;
    
    /* AST_FUNCTION_DEFINITION */
    struct AST_STRUCT* function_definition_body;
    char* function_definition_name;
    struct AST_STRUCT** function_definition_args;
    size_t function_definition_args_size;

    /* AST_VARIABLE_FUNCTION_DEFINITION */
    struct AST_STRUCT* var_function_definition_body;
    char* var_function_definition_name;
    struct AST_STRUCT** var_function_definition_args;
    size_t var_function_definition_args_size;
    struct AST_STRUCT* var_function_definition_return_variable;
    
    /* AST_IF_STATEMENT */
    struct AST_STRUCT* if_body;
    struct AST_STRUCT* if_arg;
    struct AST_STRUCT* if_else;

    /* AST_RETURN */
    struct AST_STRUCT* return_value;

    /* AST_WHILE_STATEMENT */
    struct AST_STRUCT* while_body;
    struct AST_STRUCT* while_arg;
    int while_isBroken;
    
    /* AST_ELSE_STATEMENT */
    struct AST_STRUCT* else_body;
    
    /* AST_PACK_DEFINITION */
    struct AST_STRUCT* pack_definition_body;
    size_t pack_definition_body_size;
    char* pack_definition_name;
    
    /* AST_CLASS_DEFINITION */
    struct AST_STRUCT* class_definition_body;
    size_t class_definition_body_size;
    char* class_definition_name;
    
    /* AST_PACK_DEFINITION */
    char* import_statement_imp_name;
    char* import_statement_class_name;

    /* AST_STRING */
    char* string_value;
    
    /* AST_INT */
    int int_value;

    /* AST_COMPOUND */
    struct AST_STRUCT** compound_value;
    size_t compound_size;
} AST_T;

AST_T* init_ast(int type);

#endif