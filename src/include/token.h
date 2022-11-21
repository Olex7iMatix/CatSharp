#ifndef CATS_TOKEN
#define CATS_TOKEN

typedef struct TOKEN_STRUCT {
    enum {
        TOKEN_ID,
        TOKEN_EQUALS,
        TOKEN_STRING,
        TOKEN_INT,
        TOKEN_SEMI,
        TOKEN_COMMA,
        TOKEN_DOT,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_EOF
    } type;

    char* value;
} token_T;

token_T* init_token(int type, char* value);

#endif