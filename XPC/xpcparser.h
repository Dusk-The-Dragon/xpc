#ifndef XPC_XPCPARSER_H
#define XPC_XPCPARSER_H

#include <string>
#include <vector>
#include <map>
#include "xpclexer.h"

// Represents a parsed expression tree node.
struct Expression {
    Token token;
    std::vector<Expression> children;

    // produce a Lisp‑style representation:
    //   (token child0 child1 ...)
    std::string toString() const;

    // friend output operator that includes token type name
    friend std::ostream& operator<<(std::ostream& os, const Expression& e);
};

class Parser {
public:
    Parser();

    // current token position
    int tp;

    // token stream to consume
    List<Token> tokens;

    // binding power table
    std::map<std::string, std::vector<float>> bp;

    // Pratt parser entry point
    Expression parseExpression(float rbp = 0.0);

private:
    std::vector<float> bP(const std::string& opp) const;
    Token advance();
    Token peek() const;
    Token peekNext() const;
    bool isValue(const Token& t) const;
};

#endif // XPC_XPCPARSER_H
