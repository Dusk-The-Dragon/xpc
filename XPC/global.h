#ifndef GLOBAL_H
#define GLOBAL_H
#include <map>
#include <limits>
#include <string>
#include "list.cpp"
enum TokenType {
    Identifier,
    Literal,
    Operator,
    Delimiter,
    Type,
    Modifier,
    otherKeyword,
    Semi,
    EndOfFile
};

const double INF = std::numeric_limits<double>::infinity();

inline std::map<std::string, std::pair<double, double>> opPrecidence = {
    {"!", {-INF, INF}},
    {"rt", {1.0, 1.1}},
    {"^", {1.0, 1.0}},
    {"*", {2.0, 2.0}},
    {"/", {2.0, 2.0}},
    {"%", {2.0, 2.0}},
    {"+", {3.0, 3.0}},
    {"-", {3.0, 3.0}},
    {"<", {5.0, 5.0}},
    {">", {5.0, 5.0}},
    {"<=", {5.0, 5.0}},
    {">=", {5.0, 5.0}},
    {"==", {5.0, 5.0}},
    {"!=", {5.0, 5.0}},
    {"&&", {6.0, 6.0}},
    {"||", {7.0, 7.0}}
};

#endif // GLOBAL_H