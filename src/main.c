#include <stdio.h>
#include <stdint.h>

#include "lexer.h"

int main(void) {
        Lexer *lexer = lexerNew("(((())))");
        while (lexerChar(lexer) != '\0') {
                Token *token = lexerNext(lexer);
                // whatever
                tokenCleanup(&token);
        }
        lexerCleanup(&lexer);
}
