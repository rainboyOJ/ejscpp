//基础的定义

#pragma once
#include <queue>
#include <string>
#include <variant>
#include <string_view>
#include <unordered_map>
#include <filesystem>

using namespace std::literals;


struct Expression;  //表达式
struct Statement;   //语句


enum ExecutionResultType { ExecNormal, ExecReturn, ExecBreak, ExecContinue };


//我们只会存这两种值 整数与字符串
struct Value {
    Value() : value{std::string("")} {} //默认是空的字符串
    Value(int v)  :value{v} {}
    Value(std::string_view v) : value{std::string(v)} {}
    Value(std::string&& v) : value{std::move(v)} {}

    //copy constructor
    Value(const Value& e) {
        value = e.value;
    }

    std::string_view get_type(){
        if(std::holds_alternative<int>(value))
            return "INT"sv;
        return "STRING"sv;
    }
    inline bool isInt(){ return std::holds_alternative<int>(value); }
    inline bool isString(){ return std::holds_alternative<std::string>(value); }

    template<typename T>
    T get(){ return std::get<T>(value); }


public:
    Value operator+(Value & v);
    
    Value operator-(Value & v){
        if( isInt() && v.isInt())
            return get<int>() - v.get<int>();
        throw "must both is INT type";
    }
    Value operator--(){
        if( isInt()){
            value = std::get<int>(value)-1;
            return get<int>()+1;
        }
        throw "must both is INT type";
    }
    Value operator++(){
        if( isInt()){
            value = std::get<int>(value)+1;
            return get<int>()-1;
        }
        throw "must both is INT type";
    }

    Value operator*(Value & v){
        if( isInt() && v.isInt())
            return get<int>() * v.get<int>();
        throw "must both is INT type";
    }
    Value operator/(Value & v){
        if( isInt() && v.isInt())
            return get<int>() / v.get<int>();
        throw "must both is INT type";
    }
    Value operator%(Value & v){
        if( isInt() && v.isInt())
            return get<int>() % v.get<int>();
        throw "must both is INT type";
    }

public:
    std::variant<int,std::string> value;
};




struct ExecResult {
    explicit ExecResult() : execType(ExecNormal) {}
    explicit ExecResult(ExecutionResultType execType) : execType(execType) {}
    explicit ExecResult(ExecutionResultType execType, Value retValue)
        : execType(execType), retValue(retValue) {}

    ExecutionResultType execType; //返回的类型
    Value retValue; //返回的结果
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
    bool isArray{false};
    std::vector<Value> arr;
};


// 上下文
class Context { 
    public:
        explicit Context() = default;
        // TODO 删除所有的变量
        virtual ~Context(){
            for (auto& e : vars) { //删除变量
                delete e.second;
            }
            for (auto& e : funcs) { //删除函数
                delete e.second;
            }

        }

        //是否有对应的变量
        bool hasVariable(const std::string& identName) {
            return (vars.find(identName) != vars.end() );
        }
        //创建一个变量
        Variable* createVariable(const std::string& identName, Value value){
            if(vars.find(identName) != vars.end() ){
                throw  identName + "vars name has exists";
            }
            auto var = new Variable();
            var->name = identName;
            var->value = std::move(value);
            vars[identName] = var;
            return var;
        }

        //得到一个变量
        Variable* getVariable(const std::string& identName){
            if( vars.find(identName) != vars.end())
                return vars[identName];
            else return nullptr;
        }

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
    using BuiltinFuncType = Value (*)(Runtime*,
            std::vector<Value>&);


    public:
    explicit Runtime(){}
    explicit Runtime(const std::string& fileName)
        :filePath{ std::filesystem::absolute(fileName) },fileParentPath{filePath.parent_path()}
    {}

    //TODO 根据stmts 删除所有的内存 delete
    virtual ~Runtime() override;

    //是否有内置的函数
    bool hasBuiltinFunction(const std::string& name) {
        return builtin.find(name) != builtin.end();
    }
    void setBuiltinFunction(const std::string& name,BuiltinFuncType f) {
        builtin[name] = f; 
    }
    //得到内置的函数
    BuiltinFuncType getBuiltinFunction(const std::string& name) {
        if( builtin.find(name) != builtin.end()){
            return builtin[name];
        }
        throw std::string("not found buildinFunciton : ") + name;
    }

    //加入语句
    void addStatement(Statement* stmt) { stmts.push_back(stmt);}
    //得到语句
    const std::vector<Statement*>& getStatements() const { return stmts; }

    std::filesystem::path filePath; //要解析的文件的路径
    std::filesystem::path fileParentPath; //要解析的文件的路径
    private:
    //内置函数
    std::unordered_map<std::string, BuiltinFuncType> builtin;
    //语句列表
    std::vector<Statement*> stmts;

};
