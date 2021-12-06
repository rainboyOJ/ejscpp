//基础的定义

#pragma once
#include <queue>
#include <variant>
#include <string_view>

using namespace std::literals;


struct Expression;  //表达式
struct Statement;   //语句


enum ExecutionResultType { ExecNormal, ExecReturn, ExecBreak, ExecContinue };


//我们只会存这两种值 整数与字符串
struct Value {
    Value() =default;
    Value(int v)  :value{v} {}
    Value(std::string_view v) : value{std::string(v)} {}
    std::variant<int,std::string> value;

    std::string_view get_type(){
        if(std::holds_alternative<int>(value))
            return "INT"sv;
        return "STRING"sv;
    }
    inline bool isInt(){ return std::holds_alternative<int>(value); }
    inline bool isString(){ return std::holds_alternative<std::string>(value); }

    template<typename T>
    T get(){ return std::get<T>(value); }
};


struct ExecResult {
    explicit ExecResult() : execType(ExecNormal) {}
    explicit ExecResult(ExecutionResultType execType) : execType(execType) {}
    explicit ExecResult(ExecutionResultType execType, Value retValue)
        : execType(execType), retValue(retValue) {}

    ExecutionResultType execType; //返回的类型
    Value retValue;
};

//代码块 多条语句组成
struct Block { 
    explicit Block() = default;
    std::vector<Statement*> stmts;
};

struct Function { //函数
    explicit Function() = default;
    ~Function() { delete block; }

    std::string name;
    std::vector<std::string> params; //参数
    Block* block{}; //代码块
    Expression* retExpr{}; // 返回的表达式
};


//变量
struct Variable {
    explicit Variable() = default;

    std::string name; //名字
    Value value;//值
};


// 上下文
class Context { 
    public:
        explicit Context() = default;
        virtual ~Context();

        //是否有对应的变量
        bool hasVariable(const std::string& identName); 
        //创建一个变量
        void createVariable(const std::string& identName, Value value);
        //得到一个变量
        std::string getVariable(const std::string& identName);

        // 加入一个函数
        void addFunction(const std::string& name, Function* f);
        //是否有函数
        bool hasFunction(const std::string& name);
        //得到一个函数
        Function* getFunction(const std::string& name); //根据名字得到函数

    private:
        std::unordered_map<std::string, Variable*> vars; //变量
        std::unordered_map<std::string, Function*> funcs; //函数
};

//运行时
class Runtime : public Context {
    //内置函数
    using BuiltinFuncType = Value (*)(Runtime*, std::deque<Context*>,
            std::vector<Value>);

    public:
    explicit Runtime();

    //是否有内置的函数
    bool hasBuiltinFunction(const std::string& name);
    //得到内置的函数
    BuiltinFuncType getBuiltinFunction(const std::string& name);

    //加入语句
    void addStatement(Statement* stmt);
    //得到语句
    std::vector<Statement*> getStatements(); 

    private:
    //内置函数
    std::unordered_map<std::string, BuiltinFuncType> builtin;
    //语句列表
    std::vector<Statement*> stmts;
};
