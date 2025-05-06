#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <locale>
#ifdef _WIN32
#include <windows.h>
#endif
#include "BetterString.h"
#include "StringUtils.h"

// 计时辅助函数
template<typename Func>
double measureExecutionTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

// 演示基本功能
void demonstrateBasicFeatures() {
    std::cout << "\n=== 基本功能演示 ===\n" << std::endl;
    
    // 构造函数
    better::String s1 = "Hello, World!";
    better::String s2("BetterString");
    better::String s3 = std::string("C++ String");
    
    std::cout << "s1: " << s1.c_str() << std::endl;
    std::cout << "s2: " << s2.c_str() << std::endl;
    std::cout << "s3: " << s3.c_str() << std::endl;
    
    // 字符串连接
    better::String s4 = s1 + " " + s2;
    std::cout << "s1 + ' ' + s2: " << s4.c_str() << std::endl;
    
    // 字符串比较
    std::cout << "s1 == s2: " << (s1 == s2 ? "true" : "false") << std::endl;
    std::cout << "s1 < s2: " << (s1 < s2 ? "true" : "false") << std::endl;
    
    // 访问单个字符
    std::cout << "s1[0]: " << s1[0] << std::endl;
    
    // 子字符串
    better::String sub = s1.substring(0, 5);
    std::cout << "s1.substring(0, 5): " << sub.c_str() << std::endl;
    
    // 查找
    size_t pos = s1.indexOf("World");
    std::cout << "s1.indexOf(\"World\"): " << pos << std::endl;
    
    // 大小写转换
    better::String upper = s1.toUpper();
    better::String lower = s1.toLower();
    std::cout << "s1.toUpper(): " << upper.c_str() << std::endl;
    std::cout << "s1.toLower(): " << lower.c_str() << std::endl;
    
    // 修剪
    better::String trimTest = "  空格测试  ";
    std::cout << "原始字符串: '" << trimTest.c_str() << "'" << std::endl;
    std::cout << "修剪后: '" << trimTest.trim().c_str() << "'" << std::endl;
}

// 演示高级功能
void demonstrateAdvancedFeatures() {
    std::cout << "\n=== 高级功能演示 ===\n" << std::endl;
    
    // 分割与连接
    better::String csv = "apple,banana,cherry,date";
    std::vector<better::String> fruits = csv.split(",");
    
    std::cout << "分割结果:" << std::endl;
    for (const auto& fruit : fruits) {
        std::cout << "  - " << fruit.c_str() << std::endl;
    }
    
    better::String joined = better::String::join(fruits, " | ");
    std::cout << "连接结果: " << joined.c_str() << std::endl;
    
    // 格式化
    better::String formatted = better::String::format("Pi: %.2f, Int: %d", 3.14159, 42);
    std::cout << "格式化: " << formatted.c_str() << std::endl;
    
    // 正则表达式
    better::String email = "user@example.com";
    bool isEmail = email.matches("[\\w._%+-]+@[\\w.-]+\\.[\\w]{2,}");
    std::cout << "是否为邮箱: " << (isEmail ? "是" : "否") << std::endl;
    
    better::String phoneNumber = "Phone: (123) 456-7890";
    better::String formatted_phone = phoneNumber.regexReplace("\\(([0-9]{3})\\) ([0-9]{3})-([0-9]{4})", "$1-$2-$3");
    std::cout << "正则替换: " << formatted_phone.c_str() << std::endl;
    
    // 重复与反转
    better::String star = "*";
    std::cout << "重复: " << star.repeat(10).c_str() << std::endl;
    
    better::String palindrome = "level";
    std::cout << "反转: " << palindrome.reverse().c_str() << std::endl;
    std::cout << "是否回文: " << (better::StringUtils::isPalindrome(palindrome) ? "是" : "否") << std::endl;
}

// 演示工具函数
void demonstrateUtilityFunctions() {
    std::cout << "\n=== 工具函数演示 ===\n" << std::endl;
    
    // 随机字符串
    better::String random = better::StringUtils::random(10);
    std::cout << "随机字符串: " << random.c_str() << std::endl;
    
    // 编辑距离与相似度
    better::String s1 = "kitten";
    better::String s2 = "sitting";
    std::cout << "编辑距离: " << better::StringUtils::levenshteinDistance(s1, s2) << std::endl;
    std::cout << "相似度: " << better::StringUtils::similarity(s1, s2) << std::endl;
    
    // 命名转换
    better::String camelCase = "hello world";
    std::cout << "驼峰命名: " << better::StringUtils::toCamelCase(camelCase, true).c_str() << std::endl;
    
    better::String snakeCase = "HelloWorld";
    std::cout << "蛇形命名: " << better::StringUtils::toSnakeCase(snakeCase).c_str() << std::endl;
    
    // 截断
    better::String longText = "这是一段非常长的文本，需要被截断以适应显示区域";
    std::cout << "截断: " << better::StringUtils::truncate(longText, 15).c_str() << std::endl;
    
    // HTML转义
    better::String html = "<div>Hello & World</div>";
    better::String escaped = better::StringUtils::escapeHtml(html);
    std::cout << "HTML转义: " << escaped.c_str() << std::endl;
    std::cout << "HTML还原: " << better::StringUtils::unescapeHtml(escaped).c_str() << std::endl;
}

// 性能比较
void comparePerformance() {
    std::cout << "\n=== 性能比较 ===\n" << std::endl;
    
    const int iterations = 100000;
    
    // 测试字符串连接
    double stdTime = measureExecutionTime([&]() {
        std::string result;
        for (int i = 0; i < iterations; ++i) {
            result += std::to_string(i);
        }
    });
    
    double betterTime = measureExecutionTime([&]() {
        better::String result;
        for (int i = 0; i < iterations; ++i) {
            result += better::String::fromInt(i);
        }
    });
    
    std::cout << "字符串连接 (" << iterations << "次):" << std::endl;
    std::cout << "  std::string: " << stdTime << "ms" << std::endl;
    std::cout << "  better::String: " << betterTime << "ms" << std::endl;
    std::cout << "  性能比: " << (stdTime / betterTime) << "x" << std::endl;
    
    // 测试子字符串
    std::string stdStr = "Hello, World! This is a test string for substring performance.";
    better::String betterStr(stdStr);
    
    stdTime = measureExecutionTime([&]() {
        for (int i = 0; i < iterations; ++i) {
            std::string sub = stdStr.substr(i % 10, 10);
        }
    });
    
    betterTime = measureExecutionTime([&]() {
        for (int i = 0; i < iterations; ++i) {
            better::String sub = betterStr.substring(i % 10, 10);
        }
    });
    
    std::cout << "\n子字符串提取 (" << iterations << "次):" << std::endl;
    std::cout << "  std::string: " << stdTime << "ms" << std::endl;
    std::cout << "  better::String: " << betterTime << "ms" << std::endl;
    std::cout << "  性能比: " << (stdTime / betterTime) << "x" << std::endl;
}

int main() {
    // 初始化UTF-8环境，解决中文乱码问题
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::setlocale(LC_ALL, ".UTF-8");
    
    std::cout << "===== BetterString 库演示 =====" << std::endl;
    
    demonstrateBasicFeatures();
    demonstrateAdvancedFeatures();
    demonstrateUtilityFunctions();
    comparePerformance();
    
    return 0;
}