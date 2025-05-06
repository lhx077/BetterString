cmake_minimum_required(VERSION 3.10)
project(BetterString VERSION 1.0.0 LANGUAGES CXX C)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译器特定设置
if(MSVC)
    # 启用多处理器编译
    add_compile_options(/MP)
    # 设置UTF-8编码
    add_compile_options(/utf-8)
    # 禁用特定警告
    add_compile_options(/wd4819)
    # 设置调试信息格式
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /DEBUG")
    # 启用增量链接
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL")
elseif(UNIX)
    # 设置UTF-8编码
    add_compile_options(-fexec-charset=UTF-8)
    add_compile_options(-finput-charset=UTF-8)
endif()

# 添加子项目
add_subdirectory(BetterString)
add_subdirectory(BetterStringCore)

# 安装目标
install(DIRECTORY BetterString/include/ DESTINATION include/BetterString)
install(DIRECTORY BetterStringCore/include/ DESTINATION include/BetterStringCore)

# 启用测试
enable_testing()

# 输出配置信息
message(STATUS "BetterString配置完成")
message(STATUS "C++标准: ${CMAKE_CXX_STANDARD}")
message(STATUS "编译器: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "支持的实现: BetterString (C++), BetterStringCore (C)")