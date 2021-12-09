#include "ast.hpp"

template<>
std::string Value::get<std::string>(){ 
    if( isInt())
        return std::to_string(std::get<int>(value));
    else return std::get<std::string>(value); 
}

Value Value::operator+(Value & v){
    if( isInt() ){
        if( v.isInt())
            return get<int>() + v.get<int>();
        else // string
            return std::to_string(get<int>()) + v.get<std::string>();
    }
    else //string
        return get<std::string>() + v.get<std::string>();
}

ExecResult ForStmt::interpret(Runtime* rt) {
    first->eval(rt);
    std::string ret;
    while(  (cond->eval(rt)).get<int>() != 0 ){
        for (const auto& e : block->stmts) { //执行内部
            auto res = e->interpret(rt);
            ret += res.retValue.get<std::string>();
        }

        last->eval(rt);
    }
    return ExecResult{ExecNormal,Value(ret)};
}

Value BinaryExpr::eval(Runtime* rt) {
        auto left = lhs->eval(rt);
        //delete lhs; //删除左边
        //lhs = nullptr; //是为不递归的被 delete
        if( rhs == nullptr) {
            if( opt == TK_MINUS)
                if( left.get_type() == "INT")
                    return -left.get<int>();
            if( opt == TK_PLUS_PLUS){
                  ++(
                  rt->getVariable(
                          dynamic_cast<IdentExpr*>(lhs)->identName
                          )->value);
                  return left;
                }
            if(TK_MINUS_MINUS){
                    --(rt->getVariable(
                          dynamic_cast<IdentExpr*>(lhs)->identName
                          )->value);
                  return left;
            }
            return left;
        }
        auto right = rhs->eval(rt);
        //delete rhs; //使用完删除它
        //rhs=nullptr;//是为不递归的被 delete
        switch(opt){
            case  TK_LOGOR:
                if( left.isInt() && left.get<int>() == 0)
                    return right;
                else if( left.isString() && left.get<std::string>().length() == 0)
                    return right;
                else
                    return left;
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
            case TK_LT:
                return left.get<int>() < right.get<int>() ? 1 : 0;
            case TK_LE:
                return left.get<int>() <= right.get<int>() ? 1 : 0;
            case TK_EQ:
                return left.get<int>() == right.get<int>() ? 1 : 0;
            case TK_NE:
                return left.get<int>() != right.get<int>() ? 1 : 0;
            case TK_GT:
                return left.get<int>() > right.get<int>() ? 1 : 0;
            case TK_GE:
                return left.get<int>() >= right.get<int>() ? 1 : 0;
            default:
                  throw "unsupport operator";
        }
    }
