#include <cstdio>
#include <string>

#include "json/json.h"
#include "lexical_analysis.h"
#include <filesystem>
#include <fstream>
#include <iostream>

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
        auto l = std::make_unique<Lexical>(argv[1]);
        char c = l->get();
        while( c !=EOF){
            std::cout << c ;
            c = l->get();
        }
    }
    std::cout  << std::endl;
    std::cout  << std::endl;
    

    Lexical lex(argv[1]);
    lex.printLex();

    return 0;
}
