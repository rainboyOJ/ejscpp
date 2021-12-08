/**
 * 解析器,把TOKEN流解析成语句流
 */

#pragma once

#include <memory>

#include "ast.hpp"
#include "lexical_analysis.h"

/**
 * 解析器
 *  用来解析各种语句
 */


// spr = share_ptr
using StatementSpr = std::shared_ptr<Statement>;
class Parser {

public:
    explicit Parser(const std::string& fileName) 
        : m_lex{fileName,line,column}
    {};

    //~Parser(); TODO

public:
    //开始解析
    void   parse(Runtime* rt); 
    //得到 运算运算符 优先级
    short  precedence(TOKEN op); 

private:
    //解析主表达式
    Expression* parsePrimaryExpr();
    //解析 一元表达式
    Expression* parseUnaryExpr();
    //解析表达式
    Expression* parseExpression( short oldPrecedence = 1 /*default to lowest precedence(0) + 1*/);
    //解析表达式语句
    ExpressionStmt* parseExpressionStmt(); 

    //IfStmt* parseIfStmt();
    ForStmt* parseForStmt();
    //ReturnStmt* parseReturnStmt();

    //解析一条语句
    Statement* parseStatement(); 
    //解析 多条语句
    std::vector<Statement*> parseStatementList();

    //解析block
    Block* parseBlock();

    //解析 参数列表
    //std::vector<std::string> parseParameterList();

    //解析 函数定义
    //Function* parseFuncDef(Context* context);

private:

    //得到下一个token
    std::tuple<TOKEN, std::string> next(){
        m_lex.parse();
        return m_lex.getCurrentToken();
    }

    inline TOKEN getCurrentToken() const {
        return std::get<TOKEN>(currentToken);
    }
    inline std::string getCurrentLexeme() const {
        return std::get<std::string>(currentToken);
    }

private:
    bool needRetValue{false};
    Lexical m_lex;
    const std::map<std::string, TOKEN> keywords;
    std::tuple<TOKEN, std::string> currentToken;
    std::fstream fs;
    int line   = 1;
    int column = 0;
};
