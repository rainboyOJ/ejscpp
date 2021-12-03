aux_source_directory(. DIR_SRCS)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
#add_executable(ejscpp ${DIR_SRCS})

add_library(ejscpp STATIC ${DIR_SRCS})
target_link_libraries(ejscpp jsoncpp_static)


## 添加测试
add_subdirectory(./test)
