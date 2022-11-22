#ifndef CATS_AST
#define CATS_AST
#include <stdlib.h>

typedef struct AST_STRUCT {
    enum {
        AST_VARIABLE_DEFINITION,
        AST_VARIABLE,
        AST_FUNCTION_DEFINITION,
        AST_PACK_DEFINITION,
        AST_CLASS_DEFINITION,
        AST_IMPORT_STATEMENT,
        AST_STRING,
        AST_FUNCTION_CALL,
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
    
    /* AST_FUNCTION_DEFINITION */
    struct AST_STRUCT* function_definition_body;
    char* function_definition_name;
    struct AST_STRUCT** function_definition_args;
    
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
    struct AST_STRUCT** import_statement_value;

    /* AST_STRING */
    char* string_value;
    
    /* AST_INT */
    char* int_value;

    /* AST_COMPOUND */
    struct AST_STRUCT** compound_value;
    size_t compound_size;
} AST_T;

AST_T* init_ast(int type);

#endif