cmake_minimum_required(VERSION 3.10)
project(BetterString_Regular VERSION 1.0.0 LANGUAGES C CXX)

# 设置C和C++标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 基本编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 选项设置
option(BETTER_STRING_BUILD_EXAMPLES "构建示例" ON)
option(BETTER_STRING_BUILD_TESTS "构建测试" OFF)
option(BETTER_STRING_USE_C_BACKEND "C++实现使用C作为后端" ON)
option(BETTER_STRING_CPP_STYLE_FOR_C "在C项目中启用C++风格API" ON)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置包含目录
include_directories(c/include cpp/include)

# 多文件版本 - 实际库
set(C_SOURCES 
    c/src/better_string.c
)

# C库
add_library(better_string_c STATIC ${C_SOURCES})
target_include_directories(better_string_c PUBLIC c/include)

# C++库
if(BETTER_STRING_USE_C_BACKEND)
    message(STATUS "C++实现使用C作为后端")
    set(CPP_SOURCES 
        cpp/src/better_string.cpp
    )
    add_library(better_string_cpp STATIC ${CPP_SOURCES})
    target_include_directories(better_string_cpp PUBLIC cpp/include)
    target_link_libraries(better_string_cpp PRIVATE better_string_c)
else()
    message(STATUS "C++使用独立实现")
    set(CPP_SOURCES 
        cpp/src/better_string_standalone.cpp
    )
    add_library(better_string_cpp STATIC ${CPP_SOURCES})
    target_include_directories(better_string_cpp PUBLIC cpp/include)
endif()

# 设置别名
add_library(better_string_reg ALIAS better_string_cpp)

# 为C项目提供C++风格API的选项
if(BETTER_STRING_CPP_STYLE_FOR_C)
    target_compile_definitions(better_string_c PUBLIC
        BETTER_STRING_CPP_STYLE
    )
endif()

# 安装库和头文件
install(TARGETS better_string_c better_string_cpp
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY c/include/ cpp/include/
    DESTINATION include/better_string
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# 构建示例
if(BETTER_STRING_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 构建测试
if(BETTER_STRING_BUILD_TESTS)
    enable_testing()
    add_subdirectory(c/tests)
    add_subdirectory(cpp/tests)
endif()

# 生成VS2022项目特定配置
if(MSVC)
    # 设置VS2022项目文件的属性
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    
    # 为VS创建特殊的解决方案文件夹
    set_target_properties(better_string_c PROPERTIES FOLDER "Libraries")
    set_target_properties(better_string_cpp PROPERTIES FOLDER "Libraries")
    
    # 如果有示例，将它们放在Examples文件夹中
    if(BETTER_STRING_BUILD_EXAMPLES)
        file(GLOB EXAMPLE_TARGETS "${CMAKE_BINARY_DIR}/examples/CMakeFiles/*.dir")
        foreach(TARGET ${EXAMPLE_TARGETS})
            get_filename_component(TARGET_NAME ${TARGET} NAME_WE)
            if(TARGET ${TARGET_NAME})
                set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Examples")
            endif()
        endforeach()
    endif()
endif()

# 输出配置信息
message(STATUS "BetterString普通版本配置摘要:")
message(STATUS "  构建示例: ${BETTER_STRING_BUILD_EXAMPLES}")
message(STATUS "  构建测试: ${BETTER_STRING_BUILD_TESTS}")
message(STATUS "  C++使用C后端: ${BETTER_STRING_USE_C_BACKEND}")
message(STATUS "  C语言使用C++风格API: ${BETTER_STRING_CPP_STYLE_FOR_C}") 