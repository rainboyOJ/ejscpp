add_executable(test test.cpp)
set_target_properties(test
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(test ejscpp)
#add_test(NAME test1
    #COMMAND "./test 1 2 3 4 5"
    #WORKING_DIRECTORY "${TEST_EJS_DIR}"
    #)

#add_custom_command(TARGET test
    #POST_BUILD
    #COMMAND test ../../../test/add1.ejscpp)
add_custom_target(runUnitTest test
    COMMAND test ./add1.ejscpp 
    DEPENDS test)
#add_custom_target(finish 
    #COMMAND test ../../../test/add1.ejscpp 
    #DEPENDS test)
 #add_custom_target(finish 
                   #COMMAND ${CMAKE_COMMAND} -E echo compile finish
                   #)
    
