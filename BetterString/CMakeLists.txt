# BetterString子项目配置
cmake_minimum_required(VERSION 3.10)

# 添加库
add_library(BetterString INTERFACE)
target_include_directories(BetterString INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 添加示例
add_executable(StringExample examples/StringExample.cpp)
target_link_libraries(StringExample BetterString)

# 添加测试
add_executable(StringTests tests/StringTests.cpp)
target_link_libraries(StringTests BetterString)
add_test(NAME StringTests COMMAND StringTests)