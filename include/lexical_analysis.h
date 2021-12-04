/**
* 语法分析器
*   - 将一个ejs文件解析成Token流
*/

#pragma once

#include <string>
#include <filesystem>
#include <map>
#include <queue>
#include <fstream>
#include "token.h"

#ifdef DEBUG_ENUM
#include <magic_enum.hpp>
#endif

class Lexical {
    using TOKEN_VALUE = std::pair<TOKEN, std::string>;
    public:
        Lexical() =delete;
        Lexical(const std::filesystem::path & ejs_file_path) : 
            line(0),colum(0),
            ejs_file_path(ejs_file_path),
            keywords({
                {"let", KW_LET},
                {"var", KW_VAR},
                {"if", KW_IF},
                {"else", KW_ELSE},
                {"while", KW_WHILE},
                {"null", KW_NULL},
                {"true", KW_TRUE},
                {"false", KW_FALSE},
                {"for", KW_FALSE},
                {"func", KW_FUNC},
                {"return", KW_RETURN},
                {"break", KW_BREAK},
                {"continue", KW_CONTINUE}}) 
            {
                fs.open(ejs_file_path);
            };
        ~Lexical(){
            fs.close();
        };
        bool parse();        //一个一个从ejs文件中取出Token
        const TOKEN_VALUE & getCurrentToken() const;//得到当前的TOken
        void printLex();    //输出各个Token
        char get() ;
        void unget(char c);
        char peek() ;
    private:
        char getNextChar();
        char peekNextChar();
        void setCurrentToken(TOKEN token);
        void setCurrentToken(TOKEN token,std::string && str);

        void buff(size_t const siz);

        int line  = 1;
        int colum = 0;

        bool in_script_tag = 0; //在script_tag内部

        TOKEN_VALUE currentToken; //当前是的Token
        std::fstream fs;
        std::deque<char> q;

        const std::map<std::string, TOKEN> keywords; //map

        const std::filesystem::path ejs_file_path;
};
