#pragma once
#include <map>
#include <iostream>
#include <unordered_map>
#include <filesystem>

template<typename T,typename... ts>
bool anyone(T c,ts... tt){
    return ((c == tt) || ...);
}

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}


template<typename... Args>
void err_log(const char * file,int line,Args... args){
    auto & os = std::cerr;
    const Color::Modifier red(Color::FG_RED);
    const Color::Modifier normal(Color::FG_DEFAULT);
    os << red ;
    os << "FILE : "<< std::filesystem::path(file).filename() << "  LINE:"<< line << std::endl;
    ((os << args), ... );
    os<< std::endl;
    os<< normal;
}

#define LOG(...) err_log(__FILE__,__LINE__,__VA_ARGS__)


#define panic(...) fprintf(stderr,__VA_ARGS__)
