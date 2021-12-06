//语法树

#pragma once
#include <queue>
#include "base.hpp"
#include "lexical_analysis.h"


struct Expression;  //表达式
struct Statement;   //语句

//基类 ast树上的点
struct AstNode {
    explicit AstNode(int line, int column) : line(line), column(column) {}
    virtual ~AstNode() = default;

    virtual std::string astString() { return "AstNode()"; }

    int line   = -1;
    int column = -1;
};



//===----------------------------------------------------------------------===//
// Expression
//===----------------------------------------------------------------------===//
//表达式是一个结点
//
struct Expression : public AstNode {
    using AstNode::AstNode;

    virtual ~Expression() = default;

    //计算结果
    virtual Value eval(Runtime* rt, std::deque<Context*> ctxChain); 

    std::string astString() override;
};

struct IntExpr : public Expression {
    explicit IntExpr(int line, int column) : Expression(line, column) {}

    int literal;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};


struct StringExpr : public Expression {
    explicit StringExpr(int line, int column) : Expression(line, column) {}

    std::string literal;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct ArrayExpr : public Expression {
    explicit ArrayExpr(int line, int column) : Expression(line, column) {}

    std::vector<Expression*> literal;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};


//标识符 表达式
struct IdentExpr : public Expression {
    explicit IdentExpr(std::string identName, int line, int column)
        : Expression(line, column), identName(std::move(identName)) {}
    std::string identName;
    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;

    std::string astString() override;
};

//下标表达式
struct IndexExpr : public Expression {
    explicit IndexExpr(int line, int column) : Expression(line, column) {}

    std::string identName;
    Expression* index;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct BinaryExpr : public Expression {
    explicit BinaryExpr(int line, int column) : Expression(line, column) {}
    Expression* lhs{};
    TOKEN opt{};
    Expression* rhs{};
    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;

    std::string astString() override;
};

struct FunCallExpr : public Expression {
    explicit FunCallExpr(int line, int column) : Expression(line, column) {}
    std::string funcName;
    std::vector<Expression*> args;
    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

//连接两个 expression
//赋值表达式
struct AssignExpr : public Expression {
    explicit AssignExpr(int line, int column) : Expression(line, column) {}

    Expression* lhs{};
    TOKEN opt;
    Expression* rhs{};

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;

    std::string astString() override;
};

//===----------------------------------------------------------------------===//
// Statement
//===----------------------------------------------------------------------===//
struct Statement : public AstNode {
    using AstNode::AstNode;

    virtual ~Statement() = default;
    virtual ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain);

    std::string astString() override;
};

struct BreakStmt : public Statement {
    explicit BreakStmt(int line, int column) : Statement(line, column) {}

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct ContinueStmt : public Statement {
    explicit ContinueStmt(int line, int column) : Statement(line, column) {}

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct ExpressionStmt : public Statement {
    explicit ExpressionStmt(Expression* expr, int line, int column)
        : Statement(line, column), expr(expr) {}

    Expression* expr{};

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct ReturnStmt : public Statement {
    explicit ReturnStmt(int line, int column) : Statement(line, column) {}

    Expression* ret{};

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct IfStmt : public Statement {
    explicit IfStmt(int line, int column) : Statement(line, column) {}

    Expression* cond{};
    Block* block{};
    Block* elseBlock{};

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct WhileStmt : public Statement {
    explicit WhileStmt(int line, int column) : Statement(line, column) {}

    Expression* cond{};
    Block* block{};

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};
