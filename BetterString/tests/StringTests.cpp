#include <iostream>
#include <cassert>
#include <string>
#include "../include/BetterString.h"
#include "../include/StringUtils.h"

#define TEST(name) void name(); std::cout << "Running test: " << #name << "... "; name(); std::cout << "PASSED" << std::endl;

// 测试基本构造函数和赋值
void testConstructionAndAssignment() {
    // 默认构造函数
    better::String s1;
    assert(s1.length() == 0);
    assert(strcmp(s1.c_str(), "") == 0);
    
    // 从C风格字符串构造
    better::String s2 = "Hello";
    assert(s2.length() == 5);
    assert(strcmp(s2.c_str(), "Hello") == 0);
    
    // 从std::string构造
    std::string stdStr = "World";
    better::String s3(stdStr);
    assert(s3.length() == 5);
    assert(strcmp(s3.c_str(), "World") == 0);
    
    // 复制构造函数
    better::String s4 = s2;
    assert(s4.length() == 5);
    assert(strcmp(s4.c_str(), "Hello") == 0);
    
    // 移动构造函数
    better::String s5 = std::move(s4);
    assert(s5.length() == 5);
    assert(strcmp(s5.c_str(), "Hello") == 0);
    
    // 复制赋值运算符
    better::String s6;
    s6 = s3;
    assert(s6.length() == 5);
    assert(strcmp(s6.c_str(), "World") == 0);
    
    // 移动赋值运算符
    better::String s7;
    s7 = std::move(s6);
    assert(s7.length() == 5);
    assert(strcmp(s7.c_str(), "World") == 0);
}

// 测试字符串操作
void testStringOperations() {
    better::String s1 = "Hello";
    better::String s2 = "World";
    
    // 连接
    better::String s3 = s1 + " " + s2;
    assert(strcmp(s3.c_str(), "Hello World") == 0);
    
    // 追加
    better::String s4 = "Hello";
    s4 += " World";
    assert(strcmp(s4.c_str(), "Hello World") == 0);
    
    // 比较
    assert(s1 != s2);
    assert(s1 < s2);  // 'H' < 'W'
    assert(s2 > s1);
    assert(s3 != s1);
    
    // 索引访问
    assert(s1[0] == 'H');
    assert(s1[4] == 'o');
    
    // 子字符串
    better::String sub = s3.substring(0, 5);
    assert(strcmp(sub.c_str(), "Hello") == 0);
    
    // 查找
    assert(s3.indexOf("World") == 6);
    assert(s3.indexOf("Xyz") == std::string::npos);
    assert(s3.contains("ello"));
    assert(!s3.contains("xyz"));
    
    // 前缀和后缀
    assert(s3.startsWith("Hello"));
    assert(!s3.startsWith("World"));
    assert(s3.endsWith("World"));
    assert(!s3.endsWith("Hello"));
}

// 测试字符串转换
void testStringTransformations() {
    better::String s = "Hello World";
    
    // 大小写转换
    better::String upper = s.toUpper();
    better::String lower = s.toLower();
    assert(strcmp(upper.c_str(), "HELLO WORLD") == 0);
    assert(strcmp(lower.c_str(), "hello world") == 0);
    
    // 修剪
    better::String withSpaces = "  Hello  ";
    better::String trimmed = withSpaces.trim();
    assert(strcmp(trimmed.c_str(), "Hello") == 0);
    
    // 替换
    better::String replaced = s.replace("World", "C++");
    assert(strcmp(replaced.c_str(), "Hello C++") == 0);
    
    // 分割
    better::String csv = "a,b,c,d";
    std::vector<better::String> parts = csv.split(",");
    assert(parts.size() == 4);
    assert(strcmp(parts[0].c_str(), "a") == 0);
    assert(strcmp(parts[3].c_str(), "d") == 0);
    
    // 连接
    better::String joined = better::String::join(parts, ";");
    assert(strcmp(joined.c_str(), "a;b;c;d") == 0);
}

// 测试高级功能
void testAdvancedFeatures() {
    // 格式化
    better::String formatted = better::String::format("%d + %.1f = %.1f", 5, 3.5, 8.5);
    assert(strcmp(formatted.c_str(), "5 + 3.5 = 8.5") == 0);
    
    // 正则表达式
    better::String email = "user@example.com";
    assert(email.matches("[\\w._%+-]+@[\\w.-]+\\.[\\w]{2,}"));
    
    better::String phone = "Phone: (123) 456-7890";
    better::String formatted_phone = phone.regexReplace("\\(([0-9]{3})\\) ([0-9]{3})-([0-9]{4})", "$1-$2-$3");
    assert(formatted_phone.contains("123-456-7890"));
    
    // 重复
    better::String repeated = better::String("abc").repeat(3);
    assert(strcmp(repeated.c_str(), "abcabcabc") == 0);
    
    // 反转
    better::String reversed = better::String("abc").reverse();
    assert(strcmp(reversed.c_str(), "cba") == 0);
}

// 测试工具函数
void testUtilityFunctions() {
    // 编辑距离
    better::String s1 = "kitten";
    better::String s2 = "sitting";
    assert(better::StringUtils::levenshteinDistance(s1, s2) == 3);
    
    // 相似度
    double similarity = better::StringUtils::similarity(s1, s2);
    assert(similarity > 0.5 && similarity < 0.7); // 大约0.57
    
    // 命名转换
    better::String camelCase = better::StringUtils::toCamelCase("hello world", true);
    assert(strcmp(camelCase.c_str(), "HelloWorld") == 0);
    
    better::String snakeCase = better::StringUtils::toSnakeCase("HelloWorld");
    assert(strcmp(snakeCase.c_str(), "hello_world") == 0);
    
    // 回文检查
    assert(better::StringUtils::isPalindrome("level"));
    assert(!better::StringUtils::isPalindrome("hello"));
    
    // HTML转义
    better::String html = "<div>";
    better::String escaped = better::StringUtils::escapeHtml(html);
    assert(strcmp(escaped.c_str(), "&lt;div&gt;") == 0);
    
    better::String unescaped = better::StringUtils::unescapeHtml(escaped);
    assert(strcmp(unescaped.c_str(), "<div>") == 0);
}

// 测试小字符串优化
void testSmallStringOptimization() {
    // 小字符串应该使用SSO
    better::String small = "abc";
    size_t smallCapacity = small.capacity();
    
    // 大字符串应该使用堆分配
    better::String large = "This is a much longer string that should not use small string optimization";
    size_t largeCapacity = large.capacity();
    
    // 确保大字符串的容量大于小字符串
    assert(largeCapacity > smallCapacity);
    
    // 测试从小字符串转换为大字符串
    better::String growing = "Small";
    growing += " but growing bigger and bigger until it can no longer fit in the small string buffer";
    assert(growing.capacity() > smallCapacity);
}

int main() {
    std::cout << "===== BetterString 单元测试 =====" << std::endl;
    
    TEST(testConstructionAndAssignment);
    TEST(testStringOperations);
    TEST(testStringTransformations);
    TEST(testAdvancedFeatures);
    TEST(testUtilityFunctions);
    TEST(testSmallStringOptimization);
    
    std::cout << "\n所有测试通过！" << std::endl;
    return 0;
}