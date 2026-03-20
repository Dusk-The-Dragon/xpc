#include <string>
#include <iostream>
#include <regex>
#include "xpcparser.h"
using namespace std;

// global sentinel list (might be used by parser later)
List<float> NAO = {-2.0, -2.0};

// Expression::toString implementation
std::string Expression::toString() const {
    if (children.empty()) {
        return token.val;
    }
    std::string s = "(" + token.val;
    for (const auto& c : children) {
        s += " " + c.toString();
    }
    s += ")";
    return s;
}

// helper for pretty-printing with indentation
static void printExpr(std::ostream& os, const Expression& e, int indent) {
    const char* typeNames[] = {"Identifier", "Literal", "Operator",
                               "Delimiter", "Type", "Modifier",
                               "otherKeyword", ";", "EndOfFile"};
    // opening line
    os << std::string(indent, ' ') << "(" << typeNames[e.token.type] << " " << e.token.val;
    if (!e.children.empty()) {
        os << "\n";
        for (size_t i = 0; i < e.children.size(); ++i) {
            printExpr(os, e.children[i], indent + 4);
            if (i + 1 < e.children.size())
                os << "\n";
        }
        os << "\n" << std::string(indent, ' ');
    }
    os << ")";
}

// output operator for Expression that prints token type name as well
std::ostream& operator<<(std::ostream& os, const Expression& e) {
    printExpr(os, e, 0);
    return os;
}

// Parser constructor – initialize binding powers
Parser::Parser() : tp(0), bp{
    {".", {1000.0, 1000.1}},
    {"^", {50.1, 50.0}},
    {"rt", {50.1, 50.0}},
    {"*", {40.0, 40.0}},
    {"/", {40.0, 40.0}},
    {"%", {40.0, 40.0}},
    {"+", {30.0, 30.0}},
    {"-", {30.0, 30.0}},
    {"==", {20.0, 20.0}},
    {">=", {20.0, 20.0}},
    {"<=", {20.0, 20.0}},
    {">", {20.0, 20.0}},
    {"<", {20.0, 20.0}},
    {"&&", {10.0, 10.0}},
    {"||", {10.0, 10.0}},
    {"&", {10.0, 10.0}},
    {"|", {10.0, 10.0}},
    {"~", {10.0, 10.0}},
    {"~~", {10.0, 10.0}},
    {"!", {-1.0, 999.0}},
} {}

std::vector<float> Parser::bP(const std::string& opp) const {
    if (this->bp.count(opp)) {
        return this->bp.at(opp);
    }
    return {-2.0, -2.0};
}

Token Parser::advance() {
    int limit = static_cast<int>(this->tokens.size()) - 1;
    return this->tokens[min(this->tp++, limit)];
}

Token Parser::peek() const {
    int limit = static_cast<int>(this->tokens.size()) - 1;
    return this->tokens[min(this->tp, limit)];
}

Token Parser::peekNext() const {
    int limit = static_cast<int>(this->tokens.size()) - 1;
    return this->tokens[min(this->tp + 1, limit)];
}

bool Parser::isValue(const Token& t) const {
    return t.type == Identifier || t.type == Literal;
}

Expression Parser::parseExpression(float rbp) {
    Expression lbp;
    Token t = this->advance();
    if (t.type == Operator && this->bP(t.val)[0] == -1.0) {
        lbp.token = t;
        lbp.children.push_back(this->parseExpression(999.0));
    } else if (this->isValue(t)) {
        lbp.token = t;
    } else if (t.val == "(") {
        lbp = this->parseExpression();
        if (this->peek().val != ")") {
            throw runtime_error("Expected ')'");
        }
        this->advance();
    } else {
        throw runtime_error("Unexpected token: " + t.val);
    }
    
    while (true) {
        Token next = this->peek();
        if (next.type == Operator) {
            auto bp = this->bP(next.val);
            if (bp[0] < rbp) {
                break;
            }
            this->advance();
            Expression newNode;
            newNode.token = next;
            newNode.children.push_back(lbp);
            newNode.children.push_back(this->parseExpression(bp[1]));
            lbp = newNode;
        } else {
            break;
        }
    }
    return lbp;
}
