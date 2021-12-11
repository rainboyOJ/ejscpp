#include <cstdio>
#include <string>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "lexical_analysis.h"
#include "ejscpp.h"

using namespace std;

//容器 单例模式
template<typename T>
struct ShareContainer {
public:
    // 
    static ShareContainer& create(){
        static ShareContainer sc;
        return sc;
    }

    /**
     * 放入的数据
     */
    template<typename... Args>
    void push(const std::string& name,Args... arg){
        container[name] = std::make_shared<T>(std::forward<Args>(arg)...);
    }

    //得到对应的指针
    T * get(const std::string& name) {
        auto shptr = container.find(name);
        if(  shptr == container.end() )
            return nullptr;//没有找到
        return shptr->second.get();
    }
    
private:
    ShareContainer() = default;
    using SHPTR = std::shared_ptr<T>;
    std::unordered_map<std::string,SHPTR> container;
};

Variable * myVariable(const std::string& name){
    return ShareContainer<Variable>::create().get(name);
}

int main(int argc,char * argv[]){
    printf("argc = %d\n",argc);
    for(int i=0;i<=argc-1;++i){
        printf("argv[%d] : %s\n",i,argv[i]);
    }
    if( argc <= 1){
        return 0;
    }
    {
        int line,colum;
        auto l = std::make_unique<Lexical>(argv[1],line,colum);
        char c = l->get();
        while( c !=EOF){
            std::cout << c ;
            c = l->get();
        }
    }
    std::cout  << std::endl;
    std::cout  << std::endl;

    ShareContainer<Variable>::create().push("USER.pcs_site","USER.pcs_site","https://pcs.roj.ac.cn/");
    

    try {
        
        ejscpp ejs(argv[1],myVariable);
        std::cout <<  ejs.excute();
    }
    catch(std::exception & e){
        std::cerr << " Exception : " << e.what() << "\n";
    }
    catch(const char* msg){
        std::cerr << " Exception : " << msg << "\n";
    }
    catch(std::string & msg){
        std::cerr << " Exception : " << msg << "\n";
    }
    return 0;
}
