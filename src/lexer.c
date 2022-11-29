#include "./include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

lexer_T* init_lexer(char* content) {
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->content = content;
    lexer->i = 0;
    lexer->c = content[lexer->i];

    return lexer;
}

token_T* lexer_collect_int(lexer_T* lexer) {
    printf("num\n");
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (isdigit(lexer->c)) {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(TOKEN_INT, value);
}

void lexer_advance(lexer_T* lexer) {
    if (lexer->c != '\0' && lexer->i < strlen(lexer->content)) {
        lexer->i += 1;
        lexer->c = lexer->content[lexer->i];
    }
}

void lexer_skip_whitespace(lexer_T* lexer) {
    while (isspace(lexer->c) || iswspace(lexer->c) || lexer->c == 13) {
        lexer_advance(lexer);
    }
}

token_T* lexer_get_next_token(lexer_T* lexer) {
    while (lexer->c != '\0' && lexer->i < strlen(lexer->content)) {
        if (isspace(lexer->c) || iswspace(lexer->c) || lexer->c == 13) lexer_skip_whitespace(lexer);

        if (isalnum(lexer->c))
            return lexer_collect_id(lexer);

        if (lexer->c == '"') return lexer_collect_string(lexer);
        if (isdigit(lexer->c)) return lexer_collect_int(lexer);

        switch (lexer->c)
        {
            case '#': return lexer_advance_with_token(lexer, lexer_comment(lexer)); break;
            case '=': return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer))); break;
            case ';': return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, lexer_get_current_char_as_string(lexer))); break;
            case '(': return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer))); break;
            case ')': return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer))); break;
            case '{': return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACE, lexer_get_current_char_as_string(lexer))); break;
            case '}': return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACE, lexer_get_current_char_as_string(lexer))); break;
            case '.': return lexer_advance_with_token(lexer, init_token(TOKEN_DOT, lexer_get_current_char_as_string(lexer))); break;
            case ',': return lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, lexer_get_current_char_as_string(lexer))); break;
        }
    }

    return init_token(TOKEN_EOF, "\0");
}

token_T* lexer_comment(lexer_T* lexer) {
    lexer_advance(lexer);
    
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (lexer->c != ';') {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(TOKEN_COMMENT, "#");
}

token_T* lexer_collect_string(lexer_T* lexer) {
    lexer_advance(lexer);

    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (lexer->c != '"') {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(TOKEN_STRING, value);
}

token_T* lexer_collect_id(lexer_T* lexer) {
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (isalnum(lexer->c) ) {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    return init_token(TOKEN_ID, value);
}

token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token) {
    lexer_advance(lexer);

    return token;
}

char* lexer_get_current_char_as_string(lexer_T* lexer) {
    char* str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
}