#include "base.hpp"
#include "ast.hpp"
//TODO 根据stmts 删除所有的内存 delete
Runtime::~Runtime() { 
    for (auto &e : stmts) { //删除每一条语句
        delete e;
    }
}
