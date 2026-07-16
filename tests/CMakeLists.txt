add_executable(test_hello test_hello.cc)
target_link_libraries(test_hello PRIVATE c4::c4 c4_warnings)

add_test(NAME test_hello COMMAND test_hello)
