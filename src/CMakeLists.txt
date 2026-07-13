add_library(c4 STATIC c4.c)
add_library(c4::c4 ALIAS c4)

target_include_directories(c4
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(c4 PRIVATE c4_warnings)
