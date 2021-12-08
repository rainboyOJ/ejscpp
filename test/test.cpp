#include <cstdio>
#include <string>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "lexical_analysis.h"
#include "ejscpp.h"

using namespace std;

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
    

    try {
        
        ejscpp ejs(argv[1]);
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
