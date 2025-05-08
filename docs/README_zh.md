# BetterString - 高性能跨语言字符串库

BetterString是一个高性能、功能丰富的字符串处理库，同时提供C和C++实现。它旨在满足现代应用程序开发中的字符串处理需求，支持Unicode和一系列全面的字符串操作。

## 主要特性

- **跨语言支持**：同时提供C和C++版本，统一的底层实现确保一致的性能表现
- **Unicode支持**：完整支持UTF-8、UTF-16和UTF-32编码
- **现代API设计**：C++版本提供流畅的链式调用，C版本支持C++风格API
- **丰富功能**：包括搜索、替换、分割、连接、修剪等常见字符串操作
- **灵活的集成选项**：
  - 传统的静态/动态库方式（常规版本）
  - 单头文件版本，零配置集成到任何项目（单头文件版本）

## 项目结构

为了提供最大的灵活性，BetterString被组织成两个完全独立的子项目：

```
BetterString/
├── regular/           # 常规多文件版本
│   ├── c/             # C语言实现
│   │   ├── include/   # C头文件
│   │   └── src/       # C源代码
│   ├── cpp/           # C++实现
│   │   ├── include/   # C++头文件
│   │   └── src/       # C++源代码
│   └── examples/      # 常规版本示例
│
├── single_header/     # 单头文件版本
│   ├── include/       # 单头文件
│   │   ├── better_string_single.h    # C单头文件
│   │   └── better_string_single.hpp  # C++单头文件
│   └── examples/      # 单头文件版本示例
│
├── docs/              # 文档
├── generate_vs2022.bat # VS2022项目生成脚本
└── CMakeLists.txt     # 超级构建CMake文件
```

这种结构允许您根据需要选择使用常规版本或单头文件版本，两个版本完全独立。

## 功能列表

BetterString提供丰富的字符串操作功能，包括：

### 基本操作
- 字符串创建、复制和销毁
- 字符串连接和插入
- 子字符串提取
- 查找和替换
- 大小写转换（toUpper/toLower）
- 首字母大写（capitalize）
- 字符串反转（reverse）
- 字符串修剪（trim）

### 高级功能
- Unicode编码转换
- 字符串分割和连接
- 单词计数（wordCount）
- 正则表达式匹配和替换
- 字符串格式化
- 数值转换

### C++特有功能
- 流操作符支持
- 方法链式API
- STL兼容接口
- 移动语义支持

## 快速开始

### 传统库方法（常规版本）

```cmake
# CMakeLists.txt
# 添加子目录方法
add_subdirectory(path/to/BetterString/regular)
target_link_libraries(your_target PRIVATE better_string)

# 或安装后使用
find_package(BetterString REQUIRED)
target_link_libraries(your_target PRIVATE better_string)
```

### 单头文件方法（C++）

```cpp
// 在一个源文件中定义实现宏
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.hpp"

// 使用示例
void example() {
    bs::String str("Hello, World!");
    str.append(" Welcome!").toUpper().capitalize();
    std::cout << str << std::endl;
    
    // 新功能示例
    int wordCount = str.wordCount();
    str.reverse();
}
```

### 单头文件方法（C - 使用C++风格API）

```c
// 在一个源文件中定义实现宏
#define BETTER_STRING_IMPLEMENTATION
#define BETTER_STRING_CPP_STYLE
#include "better_string_single.h"

// 使用示例
void example() {
    BsString str = BsNew("Hello, World!");
    BsChain_Begin(str);
    BsChain_Append(" Welcome!");
    BsChain_ToUpper();
    BsChain_Capitalize();
    BsChain_End();
    
    // 新功能示例
    int wordCount = BsWordCount(str);
    str = BsReverse(str);
    
    printf("%s\n", BsCStr(str));
    BsFree(str);
}
```

## 构建方法

### 使用CMake

#### 构建两个版本

```bash
# 从根目录构建
mkdir build && cd build
cmake ..
cmake --build .
```

#### 仅构建常规版本

```bash
# 直接从regular目录构建
mkdir build_regular && cd build_regular
cmake ../regular
cmake --build .
```

#### 仅构建单头文件版本

```bash
# 直接从single_header目录构建
mkdir build_single && cd build_single
cmake ../single_header
cmake --build .
```

### 使用Visual Studio 2022

```bash
# 运行项目的脚本，可以选择生成哪种项目类型
generate_vs2022.bat
```

该脚本将指导您选择生成常规版本、单头文件版本或两者都生成。

## 文档

有关详细的API文档和使用指南，请参阅[docs](docs/)目录：

- [API参考](docs/api_reference_zh.md)
- [单头文件使用指南](docs/single_header_usage_zh.md)
- [高级用法](docs/advanced_usage.md)

## 最近更新

- **项目重构**：完全分离单头文件版本和常规多文件版本为两个独立的子项目，结构更加清晰
- **新功能**：
  - `capitalize()` - 将每个单词的首字母大写，其余字母小写
  - `reverse()` - 反转字符串内容（支持UTF-8多字节字符）
  - `wordCount()` - 计算字符串中的单词数量

## 示例

每个版本都有自己的示例代码：

- 常规版本示例位于[regular/examples](regular/examples/)
- 单头文件版本示例位于[single_header/examples](single_header/examples/)

## 许可证

本项目采用Apache 2.0许可证授权。 