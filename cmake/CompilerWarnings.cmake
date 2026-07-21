add_library(h3_warnings INTERFACE)

target_compile_options(h3_warnings INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(h3_warnings INTERFACE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )
    target_link_options(h3_warnings INTERFACE
        -fsanitize=address,undefined
    )
endif()
