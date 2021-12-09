aux_source_directory(. DIR_SRCS)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
#add_executable(ejscpp ${DIR_SRCS})

# 生成静态库
add_library(ejscpp STATIC ${DIR_SRCS})
#target_link_libraries(ejscpp)

set_target_properties(ejscpp
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)


## 添加测试
#add_subdirectory(./test)
