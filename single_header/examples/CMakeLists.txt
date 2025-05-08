# 示例构建配置

# 创建变量存储所有示例目标
set(ALL_EXAMPLE_TARGETS)

# C 示例 (使用C++风格API)
add_executable(single_header_c_example c/single_header_example.c)
target_link_libraries(single_header_c_example PRIVATE better_string_single)
target_include_directories(single_header_c_example PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
list(APPEND ALL_EXAMPLE_TARGETS single_header_c_example)

# C 高级示例
add_executable(single_header_c_advanced_example c/advanced_example.c)
target_link_libraries(single_header_c_advanced_example PRIVATE better_string_single)
target_include_directories(single_header_c_advanced_example PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
list(APPEND ALL_EXAMPLE_TARGETS single_header_c_advanced_example)

# C++ 示例
add_executable(single_header_cpp_example cpp/single_header_example.cpp)
target_link_libraries(single_header_cpp_example PRIVATE better_string_single)
target_include_directories(single_header_cpp_example PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
list(APPEND ALL_EXAMPLE_TARGETS single_header_cpp_example)

# C++ 高级示例
add_executable(single_header_cpp_advanced_example cpp/advanced_example.cpp)
target_link_libraries(single_header_cpp_advanced_example PRIVATE better_string_single)
target_include_directories(single_header_cpp_advanced_example PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
list(APPEND ALL_EXAMPLE_TARGETS single_header_cpp_advanced_example)

# 为每个示例设置输出目录
if(ALL_EXAMPLE_TARGETS)
    set_target_properties(
        ${ALL_EXAMPLE_TARGETS}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
    )
endif() 