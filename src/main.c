#include <stdio.h>
#include <stdint.h>

#include "lexer/lexer.h"

int main(void) {
        const char *string = "function ok() { let 1 = 2; return 3 - 1 == 1; }";
        printf("%s\n\n", string);
        Lexer *lexer = lexerNew(string);
        while (lexerChar(lexer) != '\0') {
                Token *token = lexerNext(lexer);
                printf("%s\n", tokenStringify(token));
                tokenCleanup(&token);
        }
        lexerCleanup(&lexer);
}
