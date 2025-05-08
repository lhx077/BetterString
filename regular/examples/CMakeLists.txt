# 示例构建配置

# 创建变量存储所有示例目标
set(ALL_EXAMPLE_TARGETS)

# C 示例
add_executable(c_example c/basic_example.c)
target_link_libraries(c_example PRIVATE better_string_c)
list(APPEND ALL_EXAMPLE_TARGETS c_example)

# 添加C语言高级示例
add_executable(c_advanced_example c/advanced_example.c)
target_link_libraries(c_advanced_example PRIVATE better_string_c)
target_compile_definitions(c_advanced_example PRIVATE BETTER_STRING_CPP_STYLE)
list(APPEND ALL_EXAMPLE_TARGETS c_advanced_example)

# C++ 示例
add_executable(cpp_example cpp/basic_example.cpp)
target_link_libraries(cpp_example PRIVATE better_string_cpp)
list(APPEND ALL_EXAMPLE_TARGETS cpp_example)

# 添加C++高级示例
add_executable(cpp_advanced_example cpp/advanced_example.cpp)
target_link_libraries(cpp_advanced_example PRIVATE better_string_cpp)
list(APPEND ALL_EXAMPLE_TARGETS cpp_advanced_example)

# 为每个示例设置输出目录
if(ALL_EXAMPLE_TARGETS)
    set_target_properties(
        ${ALL_EXAMPLE_TARGETS}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
    )
endif() 