#ifndef _LEXER_H_
#define _LEXER_H_

typedef enum {
        TokenType_Illegal,
        TokenType_Eof,
        TokenType_Ident,
        TokenType_Int,

        TokenType_Function,
        TokenType_Let,
        TokenType_True,
        TokenType_False,
        TokenType_If,
        TokenType_Else,
        TokenType_While,
        TokenType_For,
        TokenType_Return,

        TokenType_Plus, // +
        TokenType_Minus, // -
        TokenType_Equals, // =
        TokenType_Bang, // !
        TokenType_Asterisk, // *
        TokenType_Slash, // /
        TokenType_Hashtag, // #
        TokenType_DoubleQuotes, // "
        TokenType_Apostrophe, // '
        TokenType_LT, // <
        TokenType_GT, // >
        TokenType_Ampersand, // &
        TokenType_Pipe, // |
        TokenType_Comma,  // ,
        TokenType_Dot, // .
        TokenType_Semicolon, // ;
        TokenType_Colon, // :
        TokenType_LParen,  // (
        TokenType_RParen, // )
        TokenType_LBrace, // {
        TokenType_RBrace, // }
        TokenType_LBracket, // [
        TokenType_RBracket, // ]
} TokenType;

typedef struct {
        TokenType type;
        char *literal;
} Token;

typedef struct SLexer Lexer;

Lexer *lexerNew(const char *input);
Token *lexerNext(Lexer *lexer);
char lexerChar(Lexer *lexer);
void lexerCleanup(Lexer **lexer);
Token *tokenNew(TokenType type, char *literal);
TokenType getTokenTypeFromToken(Token *token);
char *getLiteralFromToken(Token *token);
void tokenCleanup(Token **token);
const char *tokenStringify(Token *token);

#endif // _LEXER_H_
