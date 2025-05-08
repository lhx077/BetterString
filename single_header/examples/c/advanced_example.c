/**
 * advanced_example.c - BetterString单头文件版C语言高级功能演示
 * 
 * 这个示例展示了BetterString单头文件库中的高级字符串操作功能
 */

// 定义实现宏
#define BETTER_STRING_IMPLEMENTATION
// 启用C++风格API
#define BETTER_STRING_CPP_STYLE
#include "better_string_single.h"
#include <stdio.h>

int main() {
    printf("==== BetterString单头文件版 C语言高级示例 ====\n");
    
    // 创建测试字符串
    bs_string* text = bs_string_new("hello world, this is a betterstring example. 你好，世界！");
    printf("原始文本: %s\n", bs_string_c_str(text));
    
    // 使用标准C函数风格
    printf("\n== 标准C函数风格 ==\n");
    
    // 使用capitalize功能
    bs_string* capitalized = bs_string_capitalize(text);
    printf("首字母大写: %s\n", bs_string_c_str(capitalized));
    
    // 使用reverse功能
    bs_string* reversed = bs_string_reverse(text);
    printf("反转字符串: %s\n", bs_string_c_str(reversed));
    
    // UTF-8字符串反转（确保多字节字符正确处理）
    bs_string* utf8Text = bs_string_new("Unicode测试：你好，世界！");
    bs_string* reversedUtf8 = bs_string_reverse(utf8Text);
    printf("UTF-8原始文本: %s\n", bs_string_c_str(utf8Text));
    printf("UTF-8反转文本: %s\n", bs_string_c_str(reversedUtf8));
    
    // 单词计数功能
    size_t wordCount = bs_string_word_count(text);
    printf("单词数量: %zu\n", wordCount);
    
    // 释放不需要的字符串
    bs_string_free(capitalized);
    bs_string_free(reversed);
    bs_string_free(utf8Text);
    bs_string_free(reversedUtf8);
    
    // 使用C++风格API (在开头已定义BETTER_STRING_CPP_STYLE)
    printf("\n== C++风格API ==\n");
    
    // 创建新字符串
    BsString sample = BsNew("  BeTTer sTRIng LiBRary exAMple  ");
    printf("原始字符串: [%s]\n", BsCStr(sample));
    
    // 链式操作
    BsChain_Begin(sample);
    BsChain_Trim();           // 移除首尾空格
    BsChain_ToLower();        // 全部转为小写
    BsChain_Capitalize();     // 首字母大写处理
    BsChain_End();
    
    printf("处理后结果: [%s]\n", BsCStr(sample));
    printf("单词数量: %zu\n", BsWordCount(sample));
    
    // 普通操作
    BsString reversed_sample = BsReverse(sample);
    printf("反转字符串: %s\n", BsCStr(reversed_sample));
    
    // 综合示例
    BsString formatted = BsFormat("这是一个%s例子，包含%d个单词", "格式化", 5);
    printf("\n格式化示例: %s\n", BsCStr(formatted));
    
    // 释放字符串
    BsFree(sample);
    BsFree(reversed_sample);
    BsFree(formatted);
    
    // 释放主字符串
    bs_string_free(text);
    
    return 0;
} 