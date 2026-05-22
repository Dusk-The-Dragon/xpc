#ifndef XPCPARSER_H
#define XPCPARSER_H
#include "global.h"
#include "xpclexer.h"
struct ASTNode
{
    virtual ~ASTNode() = default;
};
struct AtomNode : public ASTNode
{
    std::string value;
    std::string type;
    AtomNode(std::string v) : value(v) {}
};
struct ExpressionNode : public ASTNode
{
    std::string type;
    std::vector<std::unique_ptr<ASTNode>> children;
    ExpressionNode(std::string t, std::vector<std::unique_ptr<ASTNode>> c) : type(t), children(std::move(c)) {}
};
#endif // XPCPARSER_H