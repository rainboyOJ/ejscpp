# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.0)

# 项目名称
project(ejscpp)

set(CMAKE_CXX_FLAGS "-std=c++17")
# 指定编译flag
# TODO 设定 debug 与release
if( CMAKE_BUILD_TYPE )
    string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)
    if (build_type STREQUAL debug)
        set(CMAKE_CXX_FLAGS "-std=c++17 -g")
        add_definitions(-DDEBUG)
    endif()
endif()



set(TEST_EJS_DIR ./test)

# 设置宏
#add_definitions(-D DEBUG)

# 指定include 的头文件的位置
include_directories(./include)
include_directories(./lib)
#include_directories(./jsoncpp-1.9.4/include)


option (USE_DEBUG "使用debug" ON)
message("CMAKE_SOURCE_DIR=${USE_DEBUG}")
if(USE_DEBUG)
    add_definitions(-DDEBUG_ENUM)
endif()


#set(JSONCPP_WITH_TESTS OFF)
#set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF)
#set(JSONCPP_WITH_WARNING_AS_ERROR  OFF)
##set(JSONCPP_WITH_STRICT_ISO  ON)
##set(JSONCPP_WITH_PKGCONFIG_SUPPORT "Generate and install .pc files" ON)
#set(JSONCPP_WITH_CMAKE_PACKAGE OFF)
#set(JSONCPP_WITH_EXAMPLE  OFF)
#set(BUILD_SHARED_LIBS OFF)
#set(BUILD_STATIC_LIBS ON)
#set(BUILD_OBJECT_LIBS OFF)
#add_subdirectory(./jsoncpp-1.9.4)



# 添加代码
#aux_source_directory(./src DIR_SRCS)

# 设置链接库路径
#link_directories(../jsoncpp-1.9.4/build/lib)

add_subdirectory(./src)

## 添加测试
add_subdirectory(./test)



# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量


# 添加库
#target_link_libraries(ejscpp jsoncpp_static)
