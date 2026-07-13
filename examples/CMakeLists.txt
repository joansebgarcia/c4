add_executable(c4_hello hello.c)
target_link_libraries(c4_hello PRIVATE c4::c4 c4_warnings)
