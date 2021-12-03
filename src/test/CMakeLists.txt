add_executable(test test.cpp)
target_link_libraries(test ejscpp jsoncpp_static)
add_test(NAME test1
    COMMAND "./test 1 2 3 4 5"
    WORKING_DIRECTORY "${TEST_EJS_DIR}"
    )

add_custom_command(TARGET test
    POST_BUILD
    COMMAND test ../../../test/double-quote.ejs)
    
