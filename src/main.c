#include <stdio.h>
#include <string.h>
// #include <SDL2/SDL.h>
#include "include/lexer.h"
#include "include/io.h"
#include "include/parser.h"
#include "include/visitor.h"

char* VERSION = "0.0.1 - prerelease";

int print_help() {
    printf("CatSharp help:\n");
    printf("Usage: `cats <filepath> [options]`\n");
    printf("Version: `%s`\n", VERSION);
    exit(1);
}

#include "include/toml.h"

int print_pack_info(char* pack) {
    FILE* fp;
    char errbuf[200];

    char* str;
    char* template = "libs/%s/libs.toml";

    sprintf(str, template, pack);

    fp = fopen(str, "r");

    if (!fp) {
        printf("[TOML] Error: Could not open file `%s`\n", str);
        exit(1);
    }

    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));

    if (!conf) {
        printf("[TOML] Error: Cannot parse file `%s`\n", str);
        exit(1);
    }

    toml_table_t* package = toml_table_in(conf, "package");
    if (!package) {
        printf("[TOML] Error: Missing [package]!\n");
        exit(1);
    }

    toml_datum_t packName = toml_string_in(package, "name");
    toml_datum_t packAuthor = toml_string_in(package, "author");
    toml_datum_t packVersion = toml_string_in(package, "version");

    if (!packName.ok) {
        printf("[TOML] Error: Cannot read package.name\n");
        exit(1);
    }

    if (!packAuthor.ok) {
        printf("[TOML] Error: Cannot read package.author\n");
        exit(1);
    }

    if (!packVersion.ok) {
        printf("[TOML] Error: Cannot read package.version\n");
        exit(1);
    }

    printf("=-=-=-= %s =-=-=-=\n", packName.u.s);
    printf("Name: %s@%s\n", packName.u.s, packVersion.u.s);
    printf("Author: %s\n", packAuthor.u.s);
    printf("Version: %s\n", packVersion.u.s);
    printf("=-=-=-= %s =-=-=-=\n", packName.u.s);
    
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 3) print_help();

    if (strcmp(argv[1], "run") == 0) {
        lexer_T* lexer = init_lexer(get_file_content(argv[2]));

        parser_T* parser = init_parser(lexer);
        AST_T* root = parser_parse(parser, parser->scope);
        visitor_T* visitor = init_visitor();
        visitor_visit(visitor, root, parser->scope);
    } else if (strcmp(argv[1], "help") == 0) {
        print_help();
    } else if (strcmp(argv[1], "pack_info") == 0) {
        return print_pack_info(argv[2]);
    } else if (strcmp(argv[1], "compile") == 0) {
        printf("Sorry but we don't have a compiler working : (\n");
    }

    return 0;
}