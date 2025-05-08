/**
 * advanced_example.cpp - BetterString单头文件版高级功能演示
 * 
 * 这个示例展示了BetterString单头文件库中的高级字符串操作功能
 */

// 定义实现宏
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.hpp"
#include <iostream>

int main() {
    std::cout << "==== BetterString单头文件版 高级示例 ====" << std::endl;
    
    // 创建测试字符串
    bs::String text("hello world, this is a betterstring example. 你好，世界！");
    std::cout << "原始文本: " << text << std::endl;
    
    // 使用capitalize功能
    bs::String capitalized = text;
    capitalized.capitalize();
    std::cout << "\n首字母大写: " << capitalized << std::endl;
    
    // 使用reverse功能
    bs::String reversed = text;
    reversed.reverse();
    std::cout << "\n反转字符串: " << reversed << std::endl;
    
    // UTF-8字符串反转（确保多字节字符正确处理）
    bs::String utf8Text("Unicode测试：你好，世界！");
    bs::String reversedUtf8 = utf8Text;
    reversedUtf8.reverse();
    std::cout << "\nUTF-8原始文本: " << utf8Text << std::endl;
    std::cout << "UTF-8反转文本: " << reversedUtf8 << std::endl;
    
    // 单词计数功能
    size_t wordCount = text.wordCount();
    std::cout << "\n单词数量: " << wordCount << std::endl;
    
    // 链式操作示例
    bs::String chainSample("  chain operation example  ");
    chainSample.trim().capitalize().append(" - modified");
    std::cout << "\n链式操作结果: " << chainSample << std::endl;
    
    // 综合示例
    bs::String combined("  BeTTer sTRIng LiBRary exAMple  ");
    std::cout << "\n综合示例原文: [" << combined << "]" << std::endl;
    
    combined.trim()           // 移除首尾空格
           .toLower()         // 全部转为小写
           .capitalize();     // 首字母大写处理
    
    std::cout << "综合示例结果: [" << combined << "]" << std::endl;
    std::cout << "单词数量: " << combined.wordCount() << std::endl;
    
    // 使用副本方法
    std::cout << "\n== 使用副本方法 ==" << std::endl;
    bs::String original("test copy methods");
    std::cout << "原始: " << original << std::endl;
    std::cout << "大写副本: " << original.toUpperCopy() << std::endl;
    std::cout << "首字母大写副本: " << original.capitalizeCopy() << std::endl;
    std::cout << "反转副本: " << original.reverseCopy() << std::endl;
    std::cout << "原始不变: " << original << std::endl;
    
    return 0;
} 