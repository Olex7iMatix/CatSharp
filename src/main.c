#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/io.h"
#include "include/parser.h"
#include "include/visitor.h"

char* VERSION = "0.0.1 - prerelease";

int print_help() {
    printf("CatSharp help:\n");
    printf("Usage: `cats <filepath> [options]`\n");
    printf("Version: `%s`\n", VERSION);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) return print_help();

    lexer_T* lexer = init_lexer(get_file_content(argv[1]));

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser, parser->scope);
    visitor_T* visitor = init_visitor();
    visitor_visit(visitor, root);

    return 0;
}