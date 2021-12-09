/**
* 语法分析器
*   - 将一个ejs文件解析成Token流
*/

#include "lexical_analysis.h"
#include "utils.hpp"
#include <iostream>
#include "exception.hpp"

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
        //fs >> q.back();
        q.back() = fs.get();
    }
    //if( !fs ){
        //q.emplace_back(EOF);
    //}
}

char Lexical::get() {
    if( q.size() == 0)
        buff(100);
    if(q.size() == 0) {
        throw std::string("read file reach end ") + __FILE__ + std::to_string(__LINE__);
    }
    char c = q.front();
    if( c == '\n') {
        line++;
        colum = 0;
    }
    else 
        colum++;
    q.pop_front();
    return c;
}

void Lexical::unget(char c){
    if( line == '\n')
        line--;
    else
        colum--;
    q.push_front(c);
}

char Lexical::peek() {
    if( q.size() == 0) 
        buff(100);
    return q.front();
}


void Lexical::setCurrentToken(TOKEN token){
    currentToken = std::make_pair(token ,"");
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

    //在<% 标签的外部的时候
    //不停的读取字符,把它们当成 字符串返回
    //直接 遇到 <% 或 eof
    if( in_script_tag == 0){ //在外部
        char c = get();
        if( c == EOF ) {
            setCurrentToken(TK_EOF); //token 外部的字符串
            return 0;
        }
        if(( c == '<'  && peek() == '%')){
            get();
            if( peek() == '=' ){
                get();
                setCurrentToken(TK_scriptlet_escaped, "");
            }
            else if( peek() == '-'){
                get();
                setCurrentToken(TK_scriptlet_unescaped, "");
            }
            else
                setCurrentToken(TK_scriptlet, "");
            in_script_tag = 1;
            return 1;
        }

        //普通字符串
        std::string value;
        while ( 1 ) {
            if(( c == '<'  && peek() == '%')){
                unget('<');
                break;
            }
            else if( c == EOF){
                unget(EOF);
                break;
            }
            value+=c;
            c = get();
        }
        setCurrentToken(LIT_OUT_SCRIPT_STR, std::move(value)); //token 外部的字符串
        return 1;
    }
    //在标签内部
    std::string value;
    while ( 1 ) {
        char c = get();

        // 各种各样的TOKEN的 解析 
        // 1 无用字符 消去
        while ( anyone(c, ' ','\n', '\r','\t')) {
            c = get();
        }

        if(c == '%' && peek() == '>'){
            get();
            setCurrentToken(TK_scriptend, std::move(value)); //这里不对
            break;
        }
        else if( c == EOF){ //ERROR
            LOG("最后的没有 tag 结束标记 %>"); //一种异常
            //throw excepetion
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
                c = peek();
            }
            auto idx =   keywords.find(value);
            if( idx == keywords.end())
                setCurrentToken(TK_IDENT, std::move(value));
            else if(value == "let" || value == "var"){ //忽略掉
                return parse(); //再次执行
            }
            else{
                setCurrentToken(idx->second,std::move(value));
            }
            return 1;
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
                    '(',')', '{','}', '[',']', ',', ';', '=','!' ,'<','>','|')){
            switch( c ){
                case '|': 
                    if( peek() == '|'){
                        get();
                        setCurrentToken(TK_LOGOR);  
                    }
                    break;
                case '+': 
                    if( peek() == '+'){
                        get();
                        setCurrentToken(TK_PLUS_PLUS);  
                    }
                    else
                        setCurrentToken(TK_PLUS,"");  
                    break;
                case '-':
                    if( peek() == '-'){
                        get();
                        setCurrentToken(TK_PLUS_PLUS);  
                    }
                    else
                        setCurrentToken(TK_MINUS,""); 
                    break;
                case '*': setCurrentToken(TK_TIMES,""); break;
                case '/': setCurrentToken(TK_DIV,"");   break;
                case '%': setCurrentToken(TK_MOD,"");   break;
                case '(': setCurrentToken(TK_LPAREN,"");   break;
                case ')': setCurrentToken(TK_RPAREN,"");   break;
                case '{': setCurrentToken(TK_LBRACE,"");   break;
                case '}': setCurrentToken(TK_RBRACE,"");   break;
                case '[': setCurrentToken(TK_LBRACKET,"");   break;
                case ']': setCurrentToken(TK_RBRACKET,"");   break;
                case ',': setCurrentToken(TK_COMMA,"");   break;
                case ';': setCurrentToken(TK_SEMICOLON,"");   break;
                case '<':
                        if( peek() == '='){
                            get();
                            setCurrentToken(TK_LE);
                        }
                        else
                          setCurrentToken(TK_LT);
                        break;
                case '!': 
                        if( peek() == '='){
                            get();
                            setCurrentToken(TK_NE);
                        }
                        else
                          setCurrentToken(TK_SEMICOLON);
                        break;
                case '=': 
                        if( peek() == '='){
                            get();
                            setCurrentToken(TK_EQ,"");
                        }
                        else setCurrentToken(TK_ASSIGN,"");
                        break;
                defaul: break;
            }
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
#ifdef DEBUG_ENUM
        auto token_name = magic_enum::enum_name(x);
        std::cout << x << " " <<  token_name<< " " << y << '\n';
#else
        std::cout << x << " " << y << '\n';
#endif
    }

    auto [x,y] = getCurrentToken();
#ifdef DEBUG_ENUM
    auto token_name = magic_enum::enum_name(x);
    std::cout << x << " " <<  token_name<< " " << y << '\n';
#else
    std::cout << x << " " << y << '\n';
#endif

}
