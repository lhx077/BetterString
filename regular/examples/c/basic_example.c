/**
 * basic_example.c - 演示BetterString C库的基本功能
 */

#include "../../c/include/better_string.h"
#include <stdio.h>

int main() {
    printf("BetterString C版本基本功能演示\n");
    printf("==========================\n\n");
    
    // 创建字符串
    bs_string* str = bs_string_new("Hello, 世界! 😊");
    printf("原始字符串: %s\n", bs_string_c_str(str));
    printf("字符数量: %zu\n", bs_string_length(str));
    printf("字节长度: %zu\n\n", bs_string_byte_length(str));
    
    // 字符串转换
    bs_string* upper = bs_string_to_upper(str);
    printf("转换为大写: %s\n", bs_string_c_str(upper));
    
    bs_string* lower = bs_string_to_lower(upper);
    printf("转换为小写: %s\n\n", bs_string_c_str(lower));
    
    // 字符串查找
    printf("查找操作:\n");
    bool contains = bs_string_contains_cstr(str, "世界");
    printf("是否包含'世界': %s\n", contains ? "是" : "否");
    
    int64_t pos = bs_string_find_cstr(str, "Hello", 0);
    printf("'Hello'的位置: %lld\n\n", pos);
    
    // 子字符串
    bs_string* sub = bs_string_substring(str, 7, 2);
    printf("子字符串(7, 2): %s\n\n", bs_string_c_str(sub));
    
    // 字符串操作
    bs_string* new_str = bs_string_new("BetterString ");
    new_str = bs_string_append_cstr(new_str, "is powerful!");
    printf("连接后的字符串: %s\n\n", bs_string_c_str(new_str));
    
    // 字符串分割
    bs_string* split_demo = bs_string_new("apple,banana,orange,grape");
    printf("分割前: %s\n", bs_string_c_str(split_demo));
    
    bs_string_array* parts = bs_string_split(split_demo, ",");
    printf("分割后 (使用','): \n");
    for (size_t i = 0; i < parts->length; i++) {
        printf("  部分 %zu: %s\n", i + 1, bs_string_c_str(parts->items[i]));
    }
    printf("\n");
    
    // Unicode支持
    bs_string* emoji = bs_string_new("😊 😂 🎉 🚀 💡 🌈");
    printf("Unicode支持 (emoji): %s\n", bs_string_c_str(emoji));
    printf("Emoji字符串长度: %zu\n", bs_string_length(emoji));
    printf("Emoji字节长度: %zu\n\n", bs_string_byte_length(emoji));
    
    // 中文支持
    bs_string* chinese = bs_string_new("中文字符串支持测试，包括标点符号！");
    printf("中文支持: %s\n", bs_string_c_str(chinese));
    printf("中文字符串长度: %zu\n", bs_string_length(chinese));
    printf("中文字节长度: %zu\n\n", bs_string_byte_length(chinese));
    
    // 格式化
    bs_string* formatted = bs_string_format("数字: %d, 字符串: %s", 42, "测试");
    printf("格式化字符串: %s\n\n", bs_string_c_str(formatted));
    
    // 清理资源
    bs_string_free(str);
    bs_string_free(upper);
    bs_string_free(lower);
    bs_string_free(sub);
    bs_string_free(new_str);
    bs_string_array_free(parts);
    bs_string_free(split_demo);
    bs_string_free(emoji);
    bs_string_free(chinese);
    bs_string_free(formatted);
    
    printf("所有演示完成！\n");
    
    return 0;
} 