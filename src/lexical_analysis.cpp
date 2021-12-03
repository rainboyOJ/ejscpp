/**
* 语法分析器
*   - 将一个ejs文件解析成Token流
*/

#include "lexical_analysis.h"
#include "utils.hpp"
#include <iostream>

const Lexical::TOKEN_VALUE & Lexical::getCurrentToken() const//得到当前的TOk
{
    return currentToken;
}

char Lexical::getNextChar(){
    char ret = static_cast<char>( fs.get() );
    if( ret == '\n'){
        line++;
        colum = 0;
    } colum++;
    return ret;
}

char Lexical::peekNextChar(){
    return fs.peek();
}

void Lexical::buff(size_t const siz){
    while ( fs && q.size() < siz ) { 
        q.emplace_back();
        fs >> q.back();
    }
    if( !fs ){
        q.emplace_back(EOF);
    }
}

char Lexical::get() {
    std::string ret = get(1);
    return ret[0];
}

std::string Lexical::get(size_t const siz){
    std::string ret;

    if( q.size() < siz )
        buff(siz - q.size());

    for(int i=1; !q.empty() && i<=siz ;++i){
        ret += q.front();
        q.pop_front();
    }

    return ret;
}

char Lexical::peek() {
    std::string ret = peek(1);
    return ret[0];
}

std::string Lexical::peek(size_t const siz){
    std::string ret;

    if( q.size() < siz )
        buff(siz - q.size());

    for(int i=0; i< siz && i < q.size();++i){
        ret += q[i];
    }

    return ret;
}
void Lexical::setCurrentToken(TOKEN token,std::string && str){
    currentToken = std::make_pair(token ,str);
}

/**
* 核心：不停的一个一个解析TOken
*
* @return 
*   1 解析正常
*   0 解析完毕
*/
bool Lexical::parse(){

    if( peek() == EOF){
        setCurrentToken(TK_EOF, "");
        return 0;
    }

    //在<% 标签的外部的时候
    //不停的读取字符,把它们当成 字符串返回
    //直接 遇到 <% 或 eof
    if( in_script_tag == 0){ //在外部
        std::string value;

        while ( 1 ) {
            char c = get();
            if(( c == '<'  && peek() == '%')){
                get();
                setCurrentToken(LIT_OUT_SCRIPT_STR, std::move(value)); //token 外部的字符串
                break;
            }
            else if( c == EOF){
                q.push_front(EOF);
                setCurrentToken(LIT_OUT_SCRIPT_STR, std::move(value));
                return 1;
            }
            value+=c;
        }
        in_script_tag = 1;
        return 1;
    }
    //在标签内部
    std::string value;
    while ( 1 ) {
        char c = get();
        if(c == '%' && peek() == '>'){
            get();
            setCurrentToken(KW_IF, std::move(value)); //这里不对
            break;
        }
        else if( c == EOF){ //ERROR
            LOG("最后的没有 tag 结束标记 %>"); //一种异常
            //throw excepetion
        }

        // 各种各样的TOKEN的 解析 
        // 1 无用字符 消去

        while ( anyone(c, ' ','\n', '\r','\t')) {
            c = get();
        }
        // 2 注释的消去 
        // 第一种注释 // 行注释 直接遇到 \n 或 '%>' 为止
        // 过滤掉
        if( c == '/' && peek() == '/' ){ 
            while (1) {
                if( c == '\n'){
                    break;
                }
                else if ( c == '%' && peek() == '>'){
                    get();
                    break;
                }
                get();
            }
        }

        // 第二种注释 /*
        if( c == '/' && peek() == '*' ){
            while (1) {
                if( c == '*' && peek() == '/')
                    break;
                get();
            }
        }

        // 3 整数 字面值
        if( std::isdigit(c) ){
            std::string value{c};

            while ( std::isdigit( peek() ) ) {
                value += get();
            }
            setCurrentToken(LIT_INT, std::move(value)); //整数token
            return 1;
        }
        // 4 标志符 关键字
        // 数字字符与_组成,数字不能放开头
        if( std::isalpha(c) || c == '_'){
            std::string value{c};
            auto c  = peek();
            while ( std::isalpha( c)  || std::isdigit(c) ) {
                value += get();
            }
            auto idx =   keywords.find(value);
            if( idx == keywords.end())
                setCurrentToken(TK_IDENT, std::move(value));
            else
                setCurrentToken(idx->second,std::move(value));
        }
        // 5 字符串字面值
        if( c == '"' || c  == '\''){
            char begin = c; //开始的那个字符
            std::string value{};
            auto c = peek();
            while ( c != begin ) {
                if( c == '\\' || peek() == begin) {
                    get();
                    value += begin;
                }
                else value += get();
                c = peek();
            }
            get(); //过滤最后 begin char
            setCurrentToken(LIT_STR,std::move(value));
            return 1;
        }
        // 6 各种运行符号
        // + - * / % () {} [] , ; = ==
        if( anyone(c, '+', '-', '*', '/', '%', 
                    '(',')', '{','}', '[',']', ',', ';', '=')){
            switch( c ){
                case '+': setCurrentToken(TK_PLUS,"") break;
                case '-': setCurrentToken(TK_MINUS,"") break;
                case '*': setCurrentToken(TK_TIMES,"") break;
                case '/': setCurrentToken(TK_DIV,"") break;
                case '%': setCurrentToken(TK_MOD,"") break;
                defaul: break;
            }

            get();
            return 1;
        }
        value+=c;
    }
    in_script_tag = 0;
    return 1;
}

void Lexical::printLex(){
    while ( parse() ) {
        auto [x,y] = getCurrentToken();
        std::cout << x << " " << y << std::endl;
    }
}
