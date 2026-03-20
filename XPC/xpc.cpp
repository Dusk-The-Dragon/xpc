#include <iostream>
#include "xpcparser.h"

int main() {
    // we'll parse a fixed expression and show the AST
    std::string sourceCode = "2 rt (a ^ 2 + b ^ 2)";

    // Lex the string
    Lexer myLexer{sourceCode};
    List<Token> tokens = myLexer.Lex();

    // instantiate parser and feed tokens
    Parser parser;
    parser.tokens = tokens;

    try {
        Expression ast = parser.parseExpression();
        std::cout << "AST:\n" << ast << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}