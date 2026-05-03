#ifndef XPC_XPCLEXER_H
#define XPC_XPCLEXER_H
#include "global.h"

struct Token {
    TokenType type;
    std::string val;
    friend std::ostream& operator<<(std::ostream& os, const Token& t);
};

struct Lexer {
    std::string code;
    List<Token> Lex();
};

#endif // XPC_XPCLEXER_H