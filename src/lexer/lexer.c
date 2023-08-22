#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer/lexer.h"

struct SLexer {
        const char *input;
        size_t inputLength;
        size_t position;
        size_t readPosition;
        char ch;
};

static void _lexerReadChar(Lexer *lexer);
static char _lexerPeek(Lexer *lexer);
static void _lexerSkipWhitespace(Lexer *lexer);

static const char *_lexerReadIdentity(Lexer *lexer, size_t *len);
static const char *_lexerReadNumber(Lexer *lexer, size_t *len);

static uint8_t _isLetter(char ch);
static uint8_t _isNumber(char ch);

static TokenType _getTokenTypeFromLiteral(const char *literal, size_t len);

/*
 * public funcs
 */

Lexer *lexerNew(const char *input) 
{
        size_t len = sizeof(Lexer);
        Lexer *lexer = malloc(len);

        lexer->input = input;
        lexer->inputLength = strlen(input);
        lexer->position = 0;
        lexer->readPosition = 0;
        
        _lexerReadChar(lexer);
        return lexer;
}

Token *lexerNext(Lexer *lexer)
{
        Token *tok = NULL;
        _lexerSkipWhitespace(lexer);
        switch (lexer->ch) {
        case '+':
                tok = tokenNew(TokenType_Plus, NULL);
                break;
        case '-':
                tok = tokenNew(TokenType_Minus, NULL);
                break;
        case '=':
                tok = tokenNew(TokenType_Equals, NULL);
                break;
        case '!':
                tok = tokenNew(TokenType_Bang, NULL);
                break;
        case '*':
                tok = tokenNew(TokenType_Asterisk, NULL);
                break;
        case '/':
                tok = tokenNew(TokenType_Slash, NULL);
                break;
        case '#':
                tok = tokenNew(TokenType_Hashtag, NULL);
                break;
        case '"':
                tok = tokenNew(TokenType_DoubleQuotes, NULL);
                break;
        case '\'':
                tok = tokenNew(TokenType_Apostrophe, NULL);
                break;
        case '<':
                tok = tokenNew(TokenType_LT, NULL);
                break;
        case '>':
                tok = tokenNew(TokenType_GT, NULL);
                break;
        case '&':
                tok = tokenNew(TokenType_Ampersand, NULL);
                break;
        case '|':
                tok = tokenNew(TokenType_Pipe, NULL);
                break;
        case ',':
                tok = tokenNew(TokenType_Comma, NULL);
                break;
        case '.':
                tok = tokenNew(TokenType_Dot, NULL);
                break;
        case ';':
                tok = tokenNew(TokenType_Semicolon, NULL);
                break;
        case ':':
                tok = tokenNew(TokenType_Colon, NULL);
                break;
        case '(':
                tok = tokenNew(TokenType_LParen, NULL);
                break;
        case ')':
                tok = tokenNew(TokenType_RParen, NULL);
                break;
        case '{':
                tok = tokenNew(TokenType_LBrace, NULL);
                break;
        case '}':
                tok = tokenNew(TokenType_RBrace, NULL);
                break;
        case '[':
                tok = tokenNew(TokenType_LBracket, NULL);
                break;
        case ']':
                tok = tokenNew(TokenType_RBracket, NULL);
                break;
        case '\0':
                tok = tokenNew(TokenType_Eof, NULL);
                break;
        }

        if (_isLetter(lexer->ch)) {
                size_t len = 0;
                char *literal = NULL;
                const char *ident = _lexerReadIdentity(lexer, &len);
                
                TokenType tokentype = _getTokenTypeFromLiteral(ident, len);
                if (tokentype == TokenType_Ident) 
                        literal = strndup(ident, len);
                tok = tokenNew(tokentype, literal);
                return tok;
        } else if (_isNumber(lexer->ch)) {
                size_t len = 0;
                char *literal = NULL;
                const char *ident = _lexerReadNumber(lexer, &len);
                
                literal = strndup(ident, len);
                tok = tokenNew(TokenType_Int, literal);
                return tok;
        }

        if (!tok) 
                tok = tokenNew(TokenType_Illegal, NULL);

        _lexerReadChar(lexer);
        return tok;
}


char lexerChar(Lexer *lexer)
{
        return lexer->ch;
}

void lexerCleanup(Lexer **lexer) 
{
        if (*lexer) 
                free(*lexer);
        *lexer = NULL;
}

Token *tokenNew(TokenType type, char *literal) 
{
        size_t len = sizeof(Token);
        Token *token = malloc(len);

        token->type = type;
        token->literal = literal;
        return token;
}

TokenType getTokenTypeFromToken(Token *token)
{
        return token->type;
}

char *getLiteralFromToken(Token *token)
{
        return token->literal;
}

void tokenCleanup(Token **token) 
{
        if (*token) {
                if ((*token)->literal)
                        free((*token)->literal);
                free(*token);
        }
        *token = NULL;
}

const char *tokenStringify(Token *token)
{
        switch (token->type) {
        case TokenType_Illegal:
                return "Illegal";
                break;
        case TokenType_Eof:
                return "Eof";
                break;
        case TokenType_Ident:
                return "Ident";
                break;
        case TokenType_Int:
                return "Int";
                break;
        case TokenType_Function:
                return "Function";
                break;
        case TokenType_Let:
                return "Let";
                break;
        case TokenType_True:
                return "True";
                break;
        case TokenType_False:
                return "False";
                break;
        case TokenType_If:
                return "If";
                break;
        case TokenType_Else:
                return "Else";
                break;
        case TokenType_While:
                return "While";
                break;
        case TokenType_For:
                return "For";
                break;
        case TokenType_Return:
                return "Return";
                break;
        case TokenType_Plus:
                return "Plus";
                break;
        case TokenType_Minus:
                return "Minus";
                break;
        case TokenType_Equals:
                return "Equals";
                break;
        case TokenType_Bang:
                return "Bang";
                break;
        case TokenType_Asterisk:
                return "Asterisk";
                break;
        case TokenType_Slash:
                return "Slash";
                break;
        case TokenType_Hashtag:
                return "Hashtag";
                break;
        case TokenType_DoubleQuotes:
                return "DoubleQuotes";
                break;
        case TokenType_Apostrophe:
                return "Apostrophe";
                break;
        case TokenType_LT:
                return "LT";
                break;
        case TokenType_GT:
                return "GT";
                break;
        case TokenType_Ampersand:
                return "Ampersand";
                break;
        case TokenType_Pipe:
                return "Pipe";
                break;
        case TokenType_Comma:
                return "Comma";
                break;
        case TokenType_Dot:
                return "Dot";
                break;
        case TokenType_Semicolon:
                return "Semicolon";
                break;
        case TokenType_Colon:
                return "Colon";
                break;
        case TokenType_LParen:
                return "LParen";
                break;
        case TokenType_RParen:
                return "RParen";
                break;
        case TokenType_LBrace:
                return "LBrace";
                break;
        case TokenType_RBrace:
                return "RBrace";
                break;
        case TokenType_LBracket:
                return "LBracket";
                break;
        case TokenType_RBracket:
                return "RBracket";
                break;
        }
        return "Unknown";
}

/*
 * private funcs
 */

static char _lexerPeek(Lexer *lexer) 
{
        if (lexer->readPosition >= lexer->inputLength) {
                return '\0';
        } else {
                return lexer->input[lexer->readPosition];
        }
}

static void _lexerReadChar(Lexer *lexer) 
{
        lexer->ch = _lexerPeek(lexer);
        lexer->position = lexer->readPosition;
        lexer->readPosition++;
}

static void _lexerSkipWhitespace(Lexer *lexer)
{
        while ((lexer->ch == ' ') || (lexer->ch == '\t') || (lexer->ch == '\n') || (lexer->ch == '\r')) 
                _lexerReadChar(lexer);
}

static const char *_lexerReadIdentity(Lexer *lexer, size_t *len)
{
        size_t old = lexer->position;
        while (_isLetter(lexer->ch)) {
                _lexerReadChar(lexer);
        }
        *len = lexer->position - old;
        return lexer->input + old;
}

static const char *_lexerReadNumber(Lexer *lexer, size_t *len) 
{
        size_t old = lexer->position;
        while (_isNumber(lexer->ch)) {
                _lexerReadChar(lexer);
        }
        *len = lexer->position - old;
        return lexer->input + old;
}

static uint8_t _isLetter(char ch) 
{
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_');
}

static uint8_t _isNumber(char ch) 
{
        return ch >= '0' && ch <= '9';
}

static TokenType _getTokenTypeFromLiteral(const char *literal, size_t len) 
{
        if (strncmp(literal, "function", len) == 0) {
                return TokenType_Function;
        } else if (strncmp(literal, "let", len) == 0) {
                return TokenType_Let;
        } else if (strncmp(literal, "true", len) == 0) {
                return TokenType_True;
        } else if (strncmp(literal, "false", len) == 0) {
                return TokenType_False;
        } else if (strncmp(literal, "if", len) == 0) {
                return TokenType_If;
        } else if (strncmp(literal, "else", len) == 0) {
                return TokenType_Else;
        } else if (strncmp(literal, "while", len) == 0) {
                return TokenType_While;
        } else if (strncmp(literal, "for", len) == 0) {
                return TokenType_For;
        } else if (strncmp(literal, "return", len) == 0) {
                return TokenType_Return;
        }
        return TokenType_Ident;
}
