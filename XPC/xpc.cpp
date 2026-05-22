#include <iostream>
#include "xpclexer.h"
#include <iostream>
int main()
{
    // we'll parse a fixed expression and show the AST
    std::string sourceCode = "(a ^ 2 + b ^ 2) ^ 0.5";

    // Lex the string
    Lexer myLexer{sourceCode};
    List<Token> tokens = myLexer.Lex();

    for (auto t : tokens)
    {
        std::cout << t;
    }

    return 0;
}