#include <fstream>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "buildinFunciton.h"

Value include(Runtime *rt,std::vector<Value> &args){
    auto path1 = fs::path(args[0].get<std::string>());
    std::string path;
    if( path1.is_absolute() )
        path = path1.string();
    else
        path = rt->fileParentPath / path1;
    if( fs::exists(path) && !fs::is_directory(path)){
        std::fstream FS(path);
        std::stringstream ss;
        ss << FS.rdbuf();
        return Value(ss.str());
    }
    return Value(path+std::string("文件不存在"));
}
