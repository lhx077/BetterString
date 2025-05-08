cmake_minimum_required(VERSION 3.10)
project(BetterString VERSION 1.0.0 LANGUAGES C CXX)

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
option(BETTER_STRING_BUILD_REGULAR "构建普通版本库" ON)
option(BETTER_STRING_BUILD_SINGLE_HEADER "构建单头文件版本库" ON)
option(BETTER_STRING_BUILD_EXAMPLES "构建示例" ON)
option(BETTER_STRING_BUILD_TESTS "构建测试" OFF)
option(BETTER_STRING_USE_C_BACKEND "C++实现使用C作为后端" ON)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置包含目录
include_directories(regular/c/include regular/cpp/include single_header/include)

# 添加子项目
if(BETTER_STRING_BUILD_REGULAR)
    add_subdirectory(regular)
endif()

if(BETTER_STRING_BUILD_SINGLE_HEADER)
    add_subdirectory(single_header)
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
    if(TARGET better_string_c)
        set_target_properties(better_string_c PROPERTIES FOLDER "Libraries")
    endif()
    
    if(TARGET better_string_cpp)
        set_target_properties(better_string_cpp PROPERTIES FOLDER "Libraries")
    endif()
    
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
message(STATUS "BetterString配置摘要:")
message(STATUS "  CMake版本: ${CMAKE_VERSION}")
message(STATUS "  C编译器: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "  C++编译器: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "  构建普通版本: ${BETTER_STRING_BUILD_REGULAR}")
message(STATUS "  构建单头文件版本: ${BETTER_STRING_BUILD_SINGLE_HEADER}")
message(STATUS "  构建示例: ${BETTER_STRING_BUILD_EXAMPLES}")
message(STATUS "  构建测试: ${BETTER_STRING_BUILD_TESTS}")
message(STATUS "  C++使用C后端: ${BETTER_STRING_USE_C_BACKEND}") 