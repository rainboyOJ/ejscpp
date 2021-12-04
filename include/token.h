/**
* Token的头文件
*/
#pragma once

enum TOKEN {
    INVALID = 0,  // <invalid>

    TK_IDENT,    // <identifier> 标识符号
    TK_EOF,      // <eof>
    LIT_INT,     // <integer>
    LIT_STR,     // <string>
    LIT_DOUBLE,  // <decimal>
    LIT_CHAR,    // <character>

    TK_BITAND,  // &
    TK_BITOR,   // |
    TK_BITNOT,  // ~
    TK_LOGAND,  // &&
    TK_LOGOR,   // ||
    TK_LOGNOT,  // !

    TK_PLUS,   // +
    TK_MINUS,  // -
    TK_TIMES,  // *
    TK_DIV,    // /
    TK_MOD,    // %

    TK_DOT,    // .

    TK_EQ,  // ==
    TK_NE,  // !=
    TK_GT,  // >
    TK_GE,  // >=
    TK_LT,  // <
    TK_LE,  // <=

    LIT_OUT_SCRIPT_STR, //script 外部的字符串
    TK_scriptlet, // <%
           //<% 'Scriptlet' tag, for control-flow, no output
            //<%_ 'Whitespace Slurping' Scriptlet tag, strips all whitespace before it
    TK_scriptlet_escaped,
            //<%= Outputs the value into the template (escaped)
    TK_scriptlet_unescaped,
            //<%- Outputs the unescaped value into the template
            //<%# Comment tag, no execution, no output
            //<%% Outputs a literal '<%'
            //%%> Outputs a literal '%>'
    TK_scriptend, // %> Plain ending tag
            //-%> Trim-mode ('newline slurp') tag, trims following newline
            //_%> 'Whitespace Slurping' ending tag, removes all whitespace after it


    TK_ASSIGN,     // =
    TK_PLUS_AGN,   // +=
    TK_MINUS_AGN,  // -=
    TK_TIMES_AGN,  // *=
    TK_DIV_AGN,    // /=
    TK_MOD_AGN,    // %=
    TK_COMMA,      // ,
    TK_LPAREN,     // (
    TK_RPAREN,     // )
    TK_LBRACE,     // {
    TK_RBRACE,     // }
    TK_LBRACKET,   // [
    TK_RBRACKET,   // ]
    TK_SEMICOLON, //;

    KW_LET,     //let
    KW_VAR,     //var
    KW_IF,        // if
    KW_ELSE,      // else
    KW_TRUE,      // true
    KW_FALSE,     // false
    KW_WHILE,     // while
    KW_FOR,       // for
    KW_NULL,      // null
    KW_FUNC,      // func
    KW_RETURN,    // return
    KW_BREAK,     // break
    KW_CONTINUE,  // continue
};

