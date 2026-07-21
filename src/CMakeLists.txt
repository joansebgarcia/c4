add_library(h3 STATIC h3.cc response.cc request.cc)
add_library(h3::h3 ALIAS h3)

target_include_directories(h3
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(h3 PRIVATE h3_warnings)
