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