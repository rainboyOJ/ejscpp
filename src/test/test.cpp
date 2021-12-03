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
    fstream fs;
    fs.open(argv[1]);
    while ( fs ) {
        cout << (char)fs.get();
    }
    cout << endl;
    cout << endl;
    cout << endl;
    fs.close();


    Lexical lex(argv[1]);
    lex.printLex();

    return 0;
}
