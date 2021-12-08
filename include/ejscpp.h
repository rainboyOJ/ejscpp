/**
 * 主头文件
 */

#pragma once

#include <memory>

#include "base.hpp"
#include "parser.h"
#include "buildinFunciton.h"
#include "exception.hpp"

/**
 * 代码 解释器
 */
class ejscpp {
public:
    
    explicit ejscpp(const std::string& filePath)
        :m_parser{filePath},rt{filePath}
    {
        rt.setBuiltinFunction("include", include);
    }
    std::string excute(); //执行器
private:
    Runtime rt; //运行时
    Parser m_parser; //语句解析器
};
