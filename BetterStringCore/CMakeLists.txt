# BetterStringCore子项目配置
cmake_minimum_required(VERSION 3.10)

# 添加库
add_library(BetterStringCore INTERFACE)
target_include_directories(BetterStringCore INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 添加优化版示例
add_executable(OptimizedStringExample examples/OptimizedStringExample.cpp)
target_link_libraries(OptimizedStringExample BetterStringCore)