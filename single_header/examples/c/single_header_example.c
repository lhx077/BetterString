/**
 * single_header_example.c - 演示BetterString单头文件版本的基本功能
 */

// 定义以启用实现部分
#define BETTER_STRING_IMPLEMENTATION
// 启用C++风格API
#define BETTER_STRING_CPP_STYLE

#include "../../include/better_string_single.h"
#include <stdio.h>

int main() {
    printf("BetterString C版本基本功能演示 (单头文件 + C++风格API)\n");
    printf("===============================================\n\n");
    
    // 使用C++风格的API创建字符串
    BsString str = BsNew("Hello, 世界! 😊");
    printf("原始字符串: %s\n", BsCStr(str));
    printf("字符数量: %zu\n", BsLength(str));
    printf("字节长度: %zu\n\n", BsByteLength(str));
    
    // 字符串转换
    BsString upper = BsToUpper(str);
    printf("转换为大写: %s\n", BsCStr(upper));
    
    BsString lower = BsToLower(upper);
    printf("转换为小写: %s\n\n", BsCStr(lower));
    
    // 字符串查找
    printf("查找操作:\n");
    bool contains = BsContainsCStr(str, "世界");
    printf("是否包含'世界': %s\n", contains ? "是" : "否");
    
    int64_t pos = BsFindCStr(str, "Hello", 0);
    printf("'Hello'的位置: %lld\n\n", pos);
    
    // 子字符串
    BsString sub = BsSubstring(str, 7, 2);
    printf("子字符串(7, 2): %s\n\n", BsCStr(sub));
    
    // 字符串操作
    BsString newStr = BsNew("BetterString ");
    newStr = BsAppendCStr(newStr, "is powerful!");
    printf("连接后的字符串: %s\n\n", BsCStr(newStr));
    
    // 使用链式操作的宏
    BsString chained;
    BsChain_Begin(BsNew("hello"));
    BsChain_Append(" world");
    BsChain_ToUpper();
    BsChain_End();
    
    printf("链式操作结果: %s\n\n", BsCStr(chained));
    
    // 字符串分割
    BsString splitDemo = BsNew("apple,banana,orange,grape");
    printf("分割前: %s\n", BsCStr(splitDemo));
    
    bs_string_array* parts = BsSplit(splitDemo, ",");
    printf("分割后 (使用','): \n");
    for (size_t i = 0; i < parts->length; i++) {
        printf("  部分 %zu: %s\n", i + 1, BsCStr(parts->items[i]));
    }
    printf("\n");
    
    // Unicode支持
    BsString emoji = BsNew("😊 😂 🎉 🚀 💡 🌈");
    printf("Unicode支持 (emoji): %s\n", BsCStr(emoji));
    printf("Emoji字符串长度: %zu\n", BsLength(emoji));
    printf("Emoji字节长度: %zu\n\n", BsByteLength(emoji));
    
    // 中文支持
    BsString chinese = BsNew("中文字符串支持测试，包括标点符号！");
    printf("中文支持: %s\n", BsCStr(chinese));
    printf("中文字符串长度: %zu\n", BsLength(chinese));
    printf("中文字节长度: %zu\n\n", BsByteLength(chinese));
    
    // 格式化
    BsString formatted = BsFormat("数字: %d, 字符串: %s", 42, "测试");
    printf("格式化字符串: %s\n\n", BsCStr(formatted));
    
    // 清理资源
    BsFree(str);
    BsFree(upper);
    BsFree(lower);
    BsFree(sub);
    BsFree(newStr);
    BsFree(chained);
    bs_string_array_free(parts);
    BsFree(splitDemo);
    BsFree(emoji);
    BsFree(chinese);
    BsFree(formatted);
    
    printf("所有演示完成！\n");
    
    return 0;
} 