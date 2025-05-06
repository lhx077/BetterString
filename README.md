# BetterString 字符串处理库
> **By:lhx077**

## 简介

BetterString 是一个高性能、功能丰富的C++字符串处理库，旨在提供类似于C#、Python等现代编程语言中字符串操作的便捷性，同时保持C++的高性能特性。该库提供了两种实现方式：

1. **BetterString** - 纯C++实现的字符串库，提供丰富的字符串操作功能
2. **BetterStringCore** - 使用C语言实现核心功能，提供更高性能的字符串处理能力

两种实现保持API完全兼容，用户可以根据需求选择使用哪种实现。

## 特性

### BetterString (C++实现)

- **现代C++支持**：充分利用移动语义等C++11特性
- **丰富的字符串操作**：提供超过30种常用字符串操作方法
- **正则表达式支持**：内置正则表达式匹配和替换功能
- **格式化功能**：类似于printf的字符串格式化
- **工具函数集**：提供额外的字符串处理工具函数
- **纯头文件实现**：易于集成到现有项目中

### BetterStringCore (C语言实现)

- **底层C实现**：核心字符串操作使用纯C实现，减少了C++抽象带来的开销
- **优化的内存管理**：改进的小字符串优化(SSO)实现，减少堆内存分配
- **高效字符串算法**：优化的字符串比较、搜索和操作算法
- **内联函数**：关键操作使用内联函数，减少函数调用开销
- **直接内存操作**：使用`memcpy`、`memcmp`等底层内存函数代替高级抽象

## 性能对比

在多种常见字符串操作上，C语言实现版本相比纯C++版本有显著的性能提升：

| 操作 | 性能提升 |
|------|----------|
| 字符串连接 | 约 1.5-2.5x |
| 字符串查找 | 约 1.3-2.0x |
| 字符串分割 | 约 1.2-1.8x |
| 内存占用 | 减少约 15-25% |

具体性能提升可能因使用场景、编译器优化和运行环境而异。

## 安装

### 使用CMake

```bash
git clone https://github.com/yourusername/BetterString.git
cd BetterString
mkdir build && cd build
cmake ..
make
make install
```

### 直接包含头文件

由于BetterString是纯头文件库，您也可以直接将相应实现的`include`目录下的文件复制到您的项目中使用。

## 使用方法

### 选择实现

您可以根据需求选择使用纯C++实现或C实现：

```cpp
// 使用纯C++实现
#include "BetterString/include/BetterString.h"

// 使用C实现（性能更高）
#include "BetterStringCore/include/BetterString.h"
```

### 基本用法

无论选择哪种实现，API保持完全一致：

```cpp
#include <iostream>
#include "BetterString/include/BetterString.h"  // 或 "BetterStringCore/include/BetterString.h"

int main() {
    // 创建字符串
    better::String str1 = "Hello, ";
    better::String str2 = "World!";
    better::String combined = str1 + str2;
    
    // 基本操作
    std::cout << "原始字符串: " << combined.c_str() << std::endl;
    std::cout << "长度: " << combined.length() << std::endl;
    std::cout << "第一个字符: " << combined[0] << std::endl;
    
    // 字符串操作
    better::String upper = combined.toUpper();
    std::cout << "大写: " << upper.c_str() << std::endl;
    
    // 查找
    size_t pos = combined.indexOf("World");
    std::cout << "'World'的位置: " << pos << std::endl;
    
    // 子字符串
    better::String sub = combined.substring(0, 5);
    std::cout << "子字符串: " << sub.c_str() << std::endl;
    
    // 分割
    better::String csv = "apple,banana,orange";
    auto parts = csv.split(better::String(","));
    std::cout << "分割结果: ";
    for (const auto& part : parts) {
        std::cout << part.c_str() << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

## 技术细节

### 小字符串优化 (SSO)

两种实现都使用了小字符串优化技术，但C语言实现版本使用更紧凑的内存布局和更高效的内存管理策略：

```c
typedef struct {
    union {
        // 小字符串直接存储
        struct {
            char buffer[BS_SSO_BUFFER_SIZE];
            uint8_t size;
            uint8_t is_small;
        } small;
        
        // 大字符串使用动态分配
        struct {
            char* data;
            size_t size;
            size_t capacity;
            uint8_t is_small;
        } large;
    } storage;
} bs_string_t;
```

### 字符串搜索优化

C实现版本使用改进的字符串搜索算法，特别是对于短字符串的搜索效率更高。

## 项目结构

```
BetterString/
├── BetterString/         # 纯C++实现
│   ├── include/
│   │   ├── BetterString.h
│   │   └── StringUtils.h
│   ├── examples/
│   └── tests/
├── BetterStringCore/     # C实现
│   ├── include/
│   │   ├── BetterString.h
│   │   └── BetterStringCore.h
│   ├── examples/
│   └── tests/
└── CMakeLists.txt        # 主CMake构建文件
```

## 编译与使用

项目使用CMake构建系统，可以分别构建两种实现或同时构建：

```bash
# 构建所有项目
cmake --build .

# 只构建纯C++实现
cmake --build . --target BetterString

# 只构建C语言实现
cmake --build . --target BetterStringCore
```

## 注意事项

1. 两种实现保持完全相同的API，但内部实现有显著差异
2. 在极少数情况下，某些边缘情况的行为可能略有不同
3. C实现版本更适合性能敏感的应用场景

## 未来计划

1. 进一步优化字符串搜索算法
2. 添加更多针对特定场景的优化
3. 考虑添加并行处理支持
...

## 贡献

欢迎提交问题报告、功能请求和代码贡献。请确保所有贡献都经过充分测试，并保持与原始API的兼容性。
