#include "ejscpp.h"


//执行器
std::string ejscpp::excute(){
    std::string res;
    m_parser.parse(&rt);
    auto stmts = rt.getStatements();
    for (auto& stmt : stmts) {
        auto astV = stmt->astString();
        std::cout << astV << std::endl;
        auto r = stmt->interpret(&rt); //解释语句
        if( stmt -> needRetValue)
            res += r.retValue.get<std::string>();
    }
    return res;
} 
