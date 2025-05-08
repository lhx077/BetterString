/**
 * single_header_example.cpp - 演示BetterString单头文件版本的C++基本功能
 */

// 定义以启用实现部分
#define BETTER_STRING_IMPLEMENTATION

#include "../../include/better_string_single.hpp"
#include <iostream>
#include <vector>

int main() {
    std::cout << "BetterString C++版本基本功能演示 (单头文件版本)" << std::endl;
    std::cout << "=======================================" << std::endl << std::endl;
    
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
    
    // 字符串操作 - 链式调用
    bs::String chainedStr("BetterString ");
    chainedStr.append("对链式操作的").append("支持很方便！");
    std::cout << "链式操作: " << chainedStr << std::endl << std::endl;
    
    // 字符串连接操作符
    bs::String concat = bs::String("可以") + "使用+" + std::string("操作符");
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
    
    // 从C API获取底层指针 (高级用法)
    bs_string* cstr = formatted.getCString();
    std::cout << "通过C API获取: " << bs_string_c_str(cstr) << std::endl << std::endl;
    
    // 无需手动释放内存，由C++的RAII自动管理
    std::cout << "所有演示完成！" << std::endl;
    
    return 0;
} 