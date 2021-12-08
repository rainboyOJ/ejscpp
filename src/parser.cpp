#include <cassert>

#include "parser.h"
#include "utils.hpp"
#include "exception.hpp"

//得到运算符 优先级
/**
 * 1  ||
 * 2  &&
 * 3  = != > < <= >=
 * 4  + - |
 * 5  * % / &
 */
short Parser::precedence(TOKEN op) {
    switch (op) {
        case TK_LOGOR: // ||
            return 1;
        case TK_LOGAND: // &&
            return 2;
        case TK_EQ: // = != > < <= >=
        case TK_NE:
        case TK_GT:
        case TK_GE:
        case TK_LT:
        case TK_LE:
            return 3;
        case TK_PLUS: // + - |
        case TK_MINUS:
        case TK_BITOR:
            return 4;
        case TK_TIMES: // * 
        case TK_MOD:
        case TK_DIV:
        case TK_BITAND:
            return 5;
        default:
            // Lowest precedence
            return 0;
    }
}



//解析 一元表达式
Expression* Parser::parseUnaryExpr() {
    if (anyone(getCurrentToken(), TK_MINUS, TK_LOGNOT, TK_BITNOT)) { //前置符号
        auto val = new BinaryExpr(line, column);
        val->opt = getCurrentToken();
        currentToken = next();
        val->lhs = parseUnaryExpr();
        return val;
    } else if (anyone(getCurrentToken(), LIT_DOUBLE, LIT_INT, LIT_STR, LIT_CHAR,
                      TK_IDENT, TK_LPAREN, TK_LBRACKET, KW_TRUE, KW_FALSE,
                      KW_NULL)) { // 字面量 () [] 标识符
        return parsePrimaryExpr();
    }
    return nullptr;
}

//解析 主表达式 
// 字面量
// 数组
Expression* Parser::parsePrimaryExpr(){
    if (getCurrentToken() == TK_IDENT) { //标识符
        auto ident = getCurrentLexeme();
        currentToken = next();
        switch (getCurrentToken()) {
            case TK_LPAREN: { //函数调用
                currentToken = next();
                //调用函数
                auto func = new FunCallExpr(1,1);
                func->funcName = ident;
                while ( getCurrentToken() !=  TK_RPAREN) {
                    func->args.push_back(parseExpression());
                    if(getCurrentToken() == TK_COMMA)
                        currentToken = next();
                }
                assert(getCurrentToken() == TK_RPAREN);
                currentToken = next();
                return func;
            }
            case TK_PLUS_PLUS: // i++，i--
            case TK_MINUS_MINUS:{
                    auto var = new BinaryExpr(line,column);
                    var->lhs = new IdentExpr(ident, line, column);
                    var->opt = getCurrentToken();
                    currentToken = next();
                    return var;
                }
            case TK_LBRACKET: { // [ //下标 TODO
                currentToken = next();
                auto* val = new IndexExpr(line, column);
                val->identName = ident;
                val->index = parseExpression();
                assert(val->index != nullptr);
                assert(getCurrentToken() == TK_RBRACKET);
                currentToken = next();
                return val;
            }
            default: {
                return new IdentExpr(ident, line, column);
            }
        }
    } else if (getCurrentToken() == LIT_INT) {
        auto val = atoi(getCurrentLexeme().c_str());
        currentToken = next();
        auto* ret = new IntExpr(line, column);
        ret->literal = val;
        return ret;
    } else if (getCurrentToken() == LIT_STR) {
        auto val = getCurrentLexeme();
        currentToken = next();
        auto* ret = new StringExpr(line, column);
        ret->literal = val;
        return ret;
    } else if (getCurrentToken() == TK_LPAREN) {
        currentToken = next();
        auto val = parseExpression();
        assert(getCurrentToken() == TK_RPAREN);
        currentToken = next();
        return val;
    } else if (getCurrentToken() == TK_LBRACKET) { // 遇到一个 [
        currentToken = next(); //下一个token
        auto* ret = new ArrayExpr(line, column); //new 一个ArrayExpr
        if (getCurrentToken() != TK_RBRACKET) { //不是 ]
            while (getCurrentToken() != TK_RBRACKET) {
                ret->literal.push_back(parseExpression());
                if (getCurrentToken() == TK_COMMA) {
                    currentToken = next();
                }
            }
            assert(getCurrentToken() == TK_RBRACKET);
            currentToken = next();
            return ret;
        } else {
            currentToken = next();
            // It's an empty array literal
            return ret;
        }
    }
    return nullptr;
}
//解析表达式
Expression* Parser::parseExpression( short oldPrecedence  ){

    auto* p = parseUnaryExpr(); //解析一元表达式

    if (anyone(getCurrentToken(), TK_ASSIGN, TK_PLUS_AGN, TK_MINUS_AGN,
               TK_TIMES_AGN, TK_DIV_AGN, TK_MOD_AGN)) {
        if (typeid(*p) != typeid(IdentExpr) &&
            typeid(*p) != typeid(IndexExpr)) {
            panic("SyntaxError: can not assign to %s", typeid(*p).name());
        }
        auto* assignExpr = new AssignExpr(line, column);
        assignExpr->opt = getCurrentToken();
        assignExpr->lhs = p;
        currentToken = next();
        assignExpr->rhs = parseExpression();
        return assignExpr;
    }


    // | +
    while (anyone(getCurrentToken(), TK_BITOR, TK_BITAND, TK_BITNOT, TK_LOGOR,
                  TK_LOGAND, TK_LOGNOT, TK_EQ, TK_NE, TK_GT, TK_GE, TK_LT,
                  TK_LE, TK_PLUS, TK_MINUS, TK_MOD, TK_TIMES, TK_DIV)) {
        // 获取当前运算符的优先级,和前一个比较
        short currentPrecedence = Parser::precedence(getCurrentToken());
        if (oldPrecedence > currentPrecedence) {
            return p;
        }
        auto tmp = new BinaryExpr(line, column);
        tmp->lhs = p;
        tmp->opt = getCurrentToken();
        currentToken = next();
        tmp->rhs = parseExpression(currentPrecedence + 1);
        p = tmp;
    }
    return p;

}

//解析表达式语句
ExpressionStmt* Parser::parseExpressionStmt() {
    ExpressionStmt* node = nullptr;
    if (auto p = parseExpression(); p != nullptr) {
        node = new ExpressionStmt(p, line, column); //创建一个表达式语句
    }
    return node;
}

//解析一条语句
Statement* Parser::parseStatement() {  
    Statement* node; 
    switch (getCurrentToken()) {
        //case KW_FOR //TODO
        case KW_FOR:
            currentToken = next(); //后面的(
            if(getCurrentToken() != TK_LPAREN ){
                throw ejscppException(line,column,"for must flow by (");
            }
            node = parseForStmt();
            node->needRetValue = true; //for必输出
            break;
        case LIT_OUT_SCRIPT_STR:{
                auto stmt = new OutStrStmt(line,column);
                stmt->value = std::move(std::get<std::string>(currentToken));
                stmt->needRetValue = true;
                node = stmt;
                currentToken = next();
                break;
            }
        case TK_scriptlet: //不需要返回值
            needRetValue = false;
            currentToken = next();
            node = new EmptyStmt(line, column);
            node->needRetValue = needRetValue;
            break;
        case TK_scriptlet_escaped: //需要返回值
        case TK_scriptlet_unescaped:
            needRetValue = true;
            currentToken = next();
            node = new EmptyStmt(line, column);
            node->needRetValue = needRetValue;
            break;
        case KW_VAR: //TODO 这两个表示定义变量,那么应该单独处理?!
        case KW_LET:
        case TK_scriptend:
            currentToken = next();
            node = new EmptyStmt(line, column);
            break;
        case KW_BREAK: //解析break
            currentToken = next();
            node = new BreakStmt(line, column);
            break;
        case KW_CONTINUE: //解析continue
            currentToken = next();
            node = new ContinueStmt(line, column);
            break;
        default:
            node = parseExpressionStmt();  //解析表达式
            if( node != nullptr)
                node->needRetValue = needRetValue;
            break;
    }
    return node;
}


//TODO for
ForStmt* Parser::parseForStmt() {
    auto* node = new ForStmt(line, column);
    currentToken = next(); // 过滤 (
    node->first = parseExpression();
    //assert(getCurrentToken() == TK_SEMICOLON); //应该是;
    if( getCurrentToken() != TK_SEMICOLON ){
        throw  ejscppException(line,column,"should be ;");
    }
    currentToken = next();
    node->cond = parseExpression();
    if( getCurrentToken() != TK_SEMICOLON ){
        throw  ejscppException(line,column,"should be ;");
    }
    currentToken = next();
    node->last = parseExpression();
    //assert(getCurrentToken() == TK_RPAREN); // )
    if( getCurrentToken() != TK_RPAREN){
        throw  ejscppException(line,column,"should be )");
    }
    currentToken = next();
    node->block = parseBlock();
    return node;
}


// 解析 block 块
Block* Parser::parseBlock() {
    Block* node{new Block};
    currentToken = next();
    node->stmts = parseStatementList(); //解析多个条语句
    assert(getCurrentToken() == TK_RBRACE); // }
    currentToken = next();
    return node;
}

std::vector<Statement*> Parser::parseStatementList() {
    std::vector<Statement*> node;
    Statement* p;
    while ((p = parseStatement()) != nullptr) {
        node.push_back(p);
    }
    return node;
}


void Parser::parse(Runtime* rt) {
    currentToken = next();
    if (getCurrentToken() == TK_EOF) {
        return; //得到EOF 就结束
    }
    do {
        rt->addStatement(parseStatement());
    } while (getCurrentToken() != TK_EOF);
}   
