cmake_minimum_required(VERSION 3.10)
project(BetterString_SingleHeader VERSION 1.0.0 LANGUAGES C CXX)

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
option(BETTER_STRING_CPP_STYLE_FOR_C "在C项目中启用C++风格API" ON)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 单头文件版本 - 接口库
add_library(better_string_single INTERFACE)
target_include_directories(better_string_single INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 设置宏以启用实现部分
target_compile_definitions(better_string_single INTERFACE
    BETTER_STRING_IMPLEMENTATION
)

# 为C项目提供C++风格API的选项
if(BETTER_STRING_CPP_STYLE_FOR_C)
    target_compile_definitions(better_string_single INTERFACE
        BETTER_STRING_CPP_STYLE
    )
endif()

# 安装单头文件
install(FILES 
    include/better_string_single.h
    include/better_string_single.hpp
    DESTINATION include/better_string
)

# 构建示例
if(BETTER_STRING_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 生成VS2022项目特定配置
if(MSVC)
    # 设置VS2022项目文件的属性
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    
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
message(STATUS "BetterString单头文件版本配置摘要:")
message(STATUS "  构建示例: ${BETTER_STRING_BUILD_EXAMPLES}")
message(STATUS "  C语言使用C++风格API: ${BETTER_STRING_CPP_STYLE_FOR_C}")