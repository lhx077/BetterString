# BetterString 单头文件版本使用指南

## 简介

为了方便集成到各种项目中，BetterString 提供了单头文件版本，不需要编译成独立的库文件，只需包含头文件即可使用所有功能。单头文件版本有以下优点：

- 不需要链接额外的库文件
- 简化项目构建过程
- 方便集成到任何C/C++项目
- 支持C++风格的API，即使在C项目中也可以使用

## 文件

- `single_header/include/better_string_single.h` - C语言单头文件版本
- `single_header/include/better_string_single.hpp` - C++单头文件版本

## 在C项目中使用

### 基本用法

```c
// 在一个源文件中定义宏以启用实现部分
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.h"

int main() {
    bs_string* str = bs_string_new("Hello, World!");
    printf("%s\n", bs_string_c_str(str));
    bs_string_free(str);
    return 0;
}
```

### 使用C++风格的API

```c
// 在一个源文件中定义宏以启用实现部分和C++风格API
#define BETTER_STRING_IMPLEMENTATION
#define BETTER_STRING_CPP_STYLE
#include "better_string_single.h"

int main() {
    BsString str = BsNew("Hello, World!");
    printf("%s\n", BsCStr(str));
    
    // 链式操作
    BsString result;
    BsChain_Begin(BsNew("hello"));
    BsChain_Append(" world");
    BsChain_ToUpper();
    BsChain_End();
    printf("%s\n", BsCStr(result));
    
    BsFree(str);
    BsFree(result);
    return 0;
}
```

## 在C++项目中使用

```cpp
// 在一个源文件中定义宏以启用实现部分
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.hpp"

int main() {
    bs::String str("Hello, World!");
    std::cout << str << std::endl;
    
    // 链式操作
    str.append(", how are you?").toUpper().trim();
    std::cout << str << std::endl;
    
    // 自动内存管理，不需要手动释放
    return 0;
}
```

## 多文件项目中使用

在多个源文件使用时，只需在一个源文件中定义`BETTER_STRING_IMPLEMENTATION`，其他文件只包含头文件即可：

```c
// 文件1: main.c
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.h"

int main() {
    // ...
}
```

```c
// 文件2: helper.c
#include "better_string_single.h"

void helper_function() {
    // ...
}
```

## 编译注意事项

### 与其他库的兼容性

如果项目中使用了其他单头文件库，建议将`BETTER_STRING_IMPLEMENTATION`定义放在一个单独的C文件中，以避免潜在的符号冲突。

### 多线程安全性

单头文件版本默认不是线程安全的。如果需要在多线程环境中使用，请在每个线程中创建独立的字符串对象，或使用外部同步机制。

### 编译优化

在Release模式下编译时，建议启用优化以获得最佳性能：

- GCC/Clang: `-O2` 或 `-O3`
- MSVC: `/O2`

## 使用CMake

提供了一个简化版的CMake文件`CMakeLists_simple.txt`，可以直接使用或作为参考：

```bash
# 拷贝或重命名CMake文件
cp CMakeLists_simple.txt CMakeLists.txt

# 构建项目
mkdir build && cd build
cmake ..
cmake --build .

# 运行示例
./single_header_c_example
./single_header_cpp_example
```

## C++风格API参考

以下是在C代码中使用C++风格API的主要宏：

| C++风格宏 | 对应的C函数 |
|----------|------------|
| BsNew(str) | bs_string_new(str) |
| BsFree(str) | bs_string_free(str) |
| BsLength(str) | bs_string_length(str) |
| BsCStr(str) | bs_string_c_str(str) |
| BsToUpper(str) | bs_string_to_upper(str) |
| BsToLower(str) | bs_string_to_lower(str) |
| BsContains(str, find) | bs_string_contains(str, find) |
| BsAppend(str, append) | bs_string_append(str, append) |
| BsSplit(str, delimiter) | bs_string_split(str, delimiter) |
| BsFormat(...) | bs_string_format(...) |
| BsCapitalize(str) | bs_string_capitalize(str) |
| BsReverse(str) | bs_string_reverse(str) |
| BsWordCount(str) | bs_string_word_count(str) |

链式操作宏：

```c
BsChain_Begin(str);
BsChain_Append("text");
BsChain_ToUpper();
BsChain_Trim();
BsChain_Capitalize();
BsChain_Reverse();
BsChain_End();
``` 