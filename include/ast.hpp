//语法树

#pragma once
#include <queue>
#include "base.hpp"
#include "lexical_analysis.h"
#include "utils.hpp"


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
    virtual Value eval(Runtime* rt) = 0;

    std::string astString() override { return "Expression()";}
};

struct IntExpr : public Expression {
    explicit IntExpr(int line, int column) : Expression(line, column) {}

    int literal;

    Value eval(Runtime* rt) override { return  literal;}
    std::string astString() override { return "IntExpr";}
};


struct StringExpr : public Expression {
    explicit StringExpr(int line, int column) : Expression(line, column) {}

    std::string literal;

    Value eval(Runtime* rt) override { return Value(literal);}
    std::string astString() override { return  "StringExpr";}
};

struct ArrayExpr : public Expression {
    explicit ArrayExpr(int line, int column) : Expression(line, column) {}

    std::vector<Expression*> literal;

    Value eval(Runtime* rt) override;
    std::string astString() override;
};


//标识符 表达式
struct IdentExpr : public Expression {
    explicit IdentExpr(std::string identName, int line, int column)
        : Expression(line, column), identName(std::move(identName)) {}
    std::string identName;
    Value eval(Runtime* rt) override {
        if( rt->hasVariable(identName) )
            return rt->getVariable(identName)->value;
        else{
            panic("error don't have variable %s",identName.c_str());
            return 0; //TODO
        }
    };

    std::string astString() override { return "IdentExpr";}
};

//下标表达式 TODO
struct IndexExpr : public Expression {
    explicit IndexExpr(int line, int column) : Expression(line, column) {}

    std::string identName;
    Expression* index;

    Value eval(Runtime* rt) override { return 0;} // TODO
    std::string astString() override { return "IndexExpr";}
};

struct BinaryExpr : public Expression {
    explicit BinaryExpr(int line, int column) : Expression(line, column) {}
    Expression* lhs{};
    TOKEN opt{};
    Expression* rhs{};
    Value eval(Runtime* rt) override {
        auto left = lhs->eval(rt);
        if( rhs == nullptr) {
            if( opt == TK_MINUS)
                if( left.get_type() == "INT")
                    return -left.get<int>();
            //TODO --
            //TODO ++
            return left;
        }
        auto right = rhs->eval(rt);
        switch(opt){
            case TK_PLUS:
                  return left + right;
            case TK_MINUS:
                  return left - right;
            case TK_TIMES:
                  return left * right;
            case TK_DIV:
                  return left / right;
            case TK_MOD:
                  return left % right;
            default:
                  throw "unsupport operator";
        }
    }

    std::string astString() override { return "BinaryExpr"; }
};


struct FunCallExpr : public Expression {
    explicit FunCallExpr(int line, int column) : Expression(line, column) {}
    std::string funcName; //函数的名字
    std::vector<Expression*> args; //参数列表
    Value eval(Runtime* rt) override {
        auto func =  rt->getBuiltinFunction(funcName) ;
        if(func != nullptr){
            std::vector<Value> v_arg;
            for (auto& e : args) {
                v_arg.push_back(e->eval(rt));
            }
            return func(rt,v_arg);
        }
        else throw  "unkown function "+funcName;
    }
    std::string astString() override { return "FunCallExpr";}
    //TODO dele all expression*
};

//连接两个 expression
//赋值表达式
struct AssignExpr : public Expression {
    explicit AssignExpr(int line, int column) : Expression(line, column) {}

    Expression* lhs{};
    TOKEN opt;
    Expression* rhs{};

    Value eval(Runtime* rt) override {
        if( typeid(*lhs) != typeid(IdentExpr)) {
            throw "left value is not Ident";
        }
        std::string & identName = dynamic_cast<IdentExpr*>(lhs)->identName;
        auto p = rt->getVariable(identName);
        if( p != nullptr){
            p->value = rhs->eval(rt);
            return p->value;
        }
        else {
            Value v = rhs->eval(rt);
            rt->createVariable(identName, v);
            return v;
        }
    }

    std::string astString() override { return "AssignExpr";}
};

//===----------------------------------------------------------------------===//
// Statement
//===----------------------------------------------------------------------===//
struct Statement : public AstNode {
    using AstNode::AstNode;

    bool needRetValue{false}; //是否需要返回字符串值
    virtual ~Statement() = default;
    virtual ExecResult interpret(Runtime* rt) = 0;

    std::string astString() override { return "Statement";}
};

struct OutStrStmt: public Statement {
    explicit OutStrStmt(int line, int column) : Statement(line, column) {}

    ExecResult interpret(Runtime* rt) override {
        return ExecResult{ExecNormal,Value(value)};
    }
    std::string value;
    std::string astString() override { return "OutStrStmt";}
};

struct EmptyStmt: public Statement {
    explicit EmptyStmt(int line, int column) : Statement(line, column) {}

    ExecResult interpret(Runtime* rt) override { 
        ExecResult r(ExecNormal);
        return r;
    }
    std::string astString() override { return "EmptyStmt";}
};

struct BreakStmt : public Statement {
    explicit BreakStmt(int line, int column) : Statement(line, column) {}

    ExecResult interpret(Runtime* rt) override {
        return ExecResult(ExecBreak);
    }
    std::string astString() override { return "BreakStmt";}
};

struct ContinueStmt : public Statement {
    explicit ContinueStmt(int line, int column) : Statement(line, column) {}

    ExecResult interpret(Runtime* rt) override {
        return  ExecResult(ExecContinue);
    }
    std::string astString() override { return  "ContinueStmt";}
};

struct ExpressionStmt : public Statement {
    explicit ExpressionStmt(Expression* expr, int line, int column)
        : Statement(line, column), expr(expr) {}

    Expression* expr{}; //表达式语句里面是一个表达式树

    ExecResult interpret(Runtime* rt) override {
        auto Val = expr->eval(rt);
        return ExecResult{ExecNormal,Val};
    }
    std::string astString() override {return "ExpressionStmt"; }
};

struct ReturnStmt : public Statement {
    explicit ReturnStmt(int line, int column) : Statement(line, column) {}

    Expression* ret{};

    ExecResult interpret(Runtime* rt) override;
    std::string astString() override;
};

struct IfStmt : public Statement {
    explicit IfStmt(int line, int column) : Statement(line, column) {}

    Expression* cond{};
    Block* block{};
    Block* elseBlock{};

    ExecResult interpret(Runtime* rt) override;
    std::string astString() override;
};

struct WhileStmt : public Statement {
    explicit WhileStmt(int line, int column) : Statement(line, column) {}

    Expression* cond{};
    Block* block{};

    ExecResult interpret(Runtime* rt) override;
    std::string astString() override;
};
