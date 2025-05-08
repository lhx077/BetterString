/**
 * basic_example.cpp - 演示BetterString C++库的基本功能
 */

#include "../../cpp/include/better_string.hpp"
#include <iostream>
#include <vector>

int main() {
    std::cout << "BetterString C++版本基本功能演示" << std::endl;
    std::cout << "=============================" << std::endl << std::endl;
    
    // 创建字符串
    bs::String str("Hello, 世界! 😊");
    std::cout << "原始字符串: " << str << std::endl;
    std::cout << "字符数量: " << str.length() << std::endl;
    std::cout << "字节长度: " << str.byteLength() << std::endl << std::endl;
    
    // 字符串转换
    bs::String upper = str.toUpperCopy();
    std::cout << "转换为大写: " << upper << std::endl;
    
    bs::String lower = upper.toLowerCopy();
    std::cout << "转换为小写: " << lower << std::endl << std::endl;
    
    // 字符串查找
    std::cout << "查找操作:" << std::endl;
    bool contains = str.contains("世界");
    std::cout << "是否包含'世界': " << (contains ? "是" : "否") << std::endl;
    
    size_t pos = str.find("Hello");
    std::cout << "'Hello'的位置: " << pos << std::endl << std::endl;
    
    // 子字符串
    bs::String sub = str.substring(7, 2);
    std::cout << "子字符串(7, 2): " << sub << std::endl << std::endl;
    
    // 字符串操作
    bs::String newStr("BetterString ");
    newStr.append("is powerful!");
    std::cout << "连接后的字符串: " << newStr << std::endl;
    
    bs::String concat = bs::String("简单") + "连接" + std::string("示例");
    std::cout << "连接示例: " << concat << std::endl << std::endl;
    
    // 字符串分割
    bs::String splitDemo("apple,banana,orange,grape");
    std::cout << "分割前: " << splitDemo << std::endl;
    
    std::vector<bs::String> parts = splitDemo.split(",");
    std::cout << "分割后 (使用','): " << std::endl;
    for (size_t i = 0; i < parts.size(); i++) {
        std::cout << "  部分 " << (i + 1) << ": " << parts[i] << std::endl;
    }
    std::cout << std::endl;
    
    // 连接
    bs::String joined = bs::String::join(parts, " | ");
    std::cout << "重新连接: " << joined << std::endl << std::endl;
    
    // Unicode支持
    bs::String emoji("😊 😂 🎉 🚀 💡 🌈");
    std::cout << "Unicode支持 (emoji): " << emoji << std::endl;
    std::cout << "Emoji字符串长度: " << emoji.length() << std::endl;
    std::cout << "Emoji字节长度: " << emoji.byteLength() << std::endl << std::endl;
    
    // 中文支持
    bs::String chinese("中文字符串支持测试，包括标点符号！");
    std::cout << "中文支持: " << chinese << std::endl;
    std::cout << "中文字符串长度: " << chinese.length() << std::endl;
    std::cout << "中文字节长度: " << chinese.byteLength() << std::endl << std::endl;
    
    // 修改字符串
    bs::String modifiable("  需要修剪的字符串  ");
    std::cout << "原始字符串: '" << modifiable << "'" << std::endl;
    modifiable.trim();
    std::cout << "修剪后: '" << modifiable << "'" << std::endl;
    
    bs::String replaceable("这是一个测试字符串，测试字符串很有用");
    std::cout << "替换前: " << replaceable << std::endl;
    replaceable.replaceAll("测试", "示例");
    std::cout << "替换后: " << replaceable << std::endl << std::endl;
    
    // 格式化
    bs::String formatted = bs::String::format("数字: %d, 字符串: %s", 42, "测试");
    std::cout << "格式化字符串: " << formatted << std::endl << std::endl;
    
    // 数值转换
    bs::String numStr = bs::String::fromInt(12345);
    std::cout << "从整数创建: " << numStr << std::endl;
    int64_t num = numStr.toInt();
    std::cout << "转换回整数: " << num << std::endl;
    
    bs::String floatStr = bs::String::fromFloat(3.14159);
    std::cout << "从浮点数创建: " << floatStr << std::endl;
    double fnum = floatStr.toFloat();
    std::cout << "转换回浮点数: " << fnum << std::endl << std::endl;
    
    // 字符串比较
    bs::String s1("测试");
    bs::String s2("测试");
    bs::String s3("不同");
    
    std::cout << "字符串比较:" << std::endl;
    std::cout << "s1 == s2: " << (s1 == s2) << std::endl;
    std::cout << "s1 == s3: " << (s1 == s3) << std::endl;
    std::cout << "s1 < s3: " << (s1 < s3) << std::endl << std::endl;
    
    std::cout << "所有演示完成！" << std::endl;
    
    return 0;
} 