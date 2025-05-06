#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <locale>
#ifdef _WIN32
#include <windows.h>
#endif
// 注意：这里只包含一个实现，避免命名冲突
#include "../include/BetterString.h" // C语言优化实现
#include "../include/StringUtils.h" // 字符串工具函数

// 计时辅助函数
template<typename Func>
double measureTime(Func func, int iterations = 1) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count() / iterations;
}

// 测试字符串连接性能
void testConcatenation() {
    std::cout << "===== 字符串连接性能测试 =====" << std::endl;
    
    // 原始实现
    double originalTime = measureTime([]() {
        better::String result;
        for (int i = 0; i < 10000; ++i) {
            result += better::String("a");
        }
    }, 5);
    
    // 优化实现
    double optimizedTime = measureTime([]() {
        better::String result;
        for (int i = 0; i < 10000; ++i) {
            result += better::String("a");
        }
    }, 5);
    
    std::cout << "原始实现: " << originalTime << "ms" << std::endl;
    std::cout << "优化实现: " << optimizedTime << "ms" << std::endl;
    std::cout << "性能提升: " << (originalTime / optimizedTime) << "x" << std::endl;
    std::cout << std::endl;
}

// 测试字符串查找性能
void testSearch() {
    std::cout << "===== 字符串查找性能测试 =====" << std::endl;
    
    // 准备测试数据
    better::String haystack("这是一个非常长的字符串，用于测试字符串查找性能。我们将在这个字符串中查找一个短语，看看优化后的实现是否更快。");
    better::String needle("短语");
    
    // 原始实现
    double originalTime = measureTime([&]() {
        for (int i = 0; i < 10000; ++i) {
            haystack.indexOf(needle);
        }
    }, 5);
    
    // 优化实现
    double optimizedTime = measureTime([&]() {
        for (int i = 0; i < 10000; ++i) {
            haystack.indexOf(needle);
        }
    }, 5);
    
    std::cout << "原始实现: " << originalTime << "ms" << std::endl;
    std::cout << "优化实现: " << optimizedTime << "ms" << std::endl;
    std::cout << "性能提升: " << (originalTime / optimizedTime) << "x" << std::endl;
    std::cout << std::endl;
}

// 测试字符串分割性能
void testSplit() {
    std::cout << "===== 字符串分割性能测试 =====" << std::endl;
    
    // 准备测试数据
    better::String text("这,是,一个,用于,测试,字符串,分割性能,的,长字符串");
    
    // 原始实现
    double originalTime = measureTime([&]() {
        for (int i = 0; i < 1000; ++i) {
            auto parts = text.split(better::String(","));
        }
    }, 5);
    
    // 优化实现
    double optimizedTime = measureTime([&]() {
        for (int i = 0; i < 1000; ++i) {
            auto parts = text.split(better::String(","));
        }
    }, 5);
    
    std::cout << "原始实现: " << originalTime << "ms" << std::endl;
    std::cout << "优化实现: " << optimizedTime << "ms" << std::endl;
    std::cout << "性能提升: " << (originalTime / optimizedTime) << "x" << std::endl;
    std::cout << std::endl;
}

// 功能演示
void demonstrateFeatures() {
    std::cout << "===== 功能演示 =====" << std::endl;
    
    // 创建字符串
    better::String str1 = "Hello, ";
    better::String str2 = "World!";
    
    // 字符串连接
    better::String combined = str1 + str2;
    std::cout << "连接结果: " << combined.c_str() << std::endl;
    
    // 字符串查找
    size_t pos = combined.indexOf("World");
    std::cout << "'World'的位置: " << pos << std::endl;
    
    // 子字符串
    better::String sub = combined.substring(0, 5);
    std::cout << "子字符串: " << sub.c_str() << std::endl;
    
    // 字符串分割
    better::String csv = "apple,banana,orange,grape";
    auto fruits = csv.split(better::String(","));
    std::cout << "分割结果: ";
    for (const auto& fruit : fruits) {
        std::cout << fruit.c_str() << " ";
    }
    std::cout << std::endl;
    
    // 字符串转换
    better::String upper = combined.toUpper();
    std::cout << "转大写: " << upper.c_str() << std::endl;
    
    // 字符串替换
    better::String replaced = combined.replace("World", "C++");
    std::cout << "替换后: " << replaced.c_str() << std::endl;
    
    std::cout << std::endl;
}

int main() {
    // 初始化UTF-8环境，解决中文乱码问题
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::setlocale(LC_ALL, ".UTF-8");
    
    // 调用BetterStringCore的UTF-8环境初始化函数
    bs_init_utf8_environment();
    
    std::cout << "BetterString 优化版本性能测试" << std::endl;
    std::cout << "============================" << std::endl << std::endl;
    
    // 功能演示
    demonstrateFeatures();
    
    // 性能测试
    testConcatenation();
    testSearch();
    testSplit();
    
    std::cout << "测试完成!" << std::endl;
    std::cout << "注意: 实际性能提升可能因编译器优化和运行环境而异" << std::endl;
    
    return 0;
}