@echo off
setlocal enabledelayedexpansion

echo BetterString - 生成Visual Studio 2022项目文件
echo ==============================================
echo.

:: 检查cmake是否可用
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo 错误: 未找到cmake。请确保cmake已安装并添加到PATH中。
    exit /b 1
)

echo 选择您要生成的项目类型:
echo 1. 普通版本（多文件库）
echo 2. 单头文件版本
echo 3. 两者都生成
echo.

set /p choice=请输入选项（1-3）: 

if "%choice%"=="1" goto :regular
if "%choice%"=="2" goto :single
if "%choice%"=="3" goto :both
echo 无效的选项，请重新运行脚本并选择有效选项。
exit /b 1

:regular
echo 正在生成普通版本的VS2022项目文件...
if not exist build_vs2022_regular mkdir build_vs2022_regular
pushd build_vs2022_regular

cmake ..\regular -G "Visual Studio 17 2022" -A x64 ^
      -DBETTER_STRING_BUILD_EXAMPLES=ON

if %ERRORLEVEL% neq 0 (
    echo 错误: 普通版本项目生成失败。
    popd
    exit /b 1
)
popd

echo.
echo 成功生成普通版本Visual Studio 2022项目文件:
echo   - build_vs2022_regular\BetterString_Regular.sln
echo.
goto :end

:single
echo 正在生成单头文件版本的VS2022项目文件...
if not exist build_vs2022_single mkdir build_vs2022_single
pushd build_vs2022_single

cmake ..\single_header -G "Visual Studio 17 2022" -A x64 ^
      -DBETTER_STRING_BUILD_EXAMPLES=ON

if %ERRORLEVEL% neq 0 (
    echo 错误: 单头文件版本项目生成失败。
    popd
    exit /b 1
)
popd

echo.
echo 成功生成单头文件版本Visual Studio 2022项目文件:
echo   - build_vs2022_single\BetterString_SingleHeader.sln
echo.
goto :end

:both
echo 正在生成普通版本的VS2022项目文件...
if not exist build_vs2022_regular mkdir build_vs2022_regular
pushd build_vs2022_regular

cmake ..\regular -G "Visual Studio 17 2022" -A x64 ^
      -DBETTER_STRING_BUILD_EXAMPLES=ON

if %ERRORLEVEL% neq 0 (
    echo 错误: 普通版本项目生成失败。
    popd
    exit /b 1
)
popd

echo 正在生成单头文件版本的VS2022项目文件...
if not exist build_vs2022_single mkdir build_vs2022_single
pushd build_vs2022_single

cmake ..\single_header -G "Visual Studio 17 2022" -A x64 ^
      -DBETTER_STRING_BUILD_EXAMPLES=ON

if %ERRORLEVEL% neq 0 (
    echo 错误: 单头文件版本项目生成失败。
    popd
    exit /b 1
)
popd

echo.
echo 成功生成Visual Studio 2022项目文件:
echo   - 普通版本: build_vs2022_regular\BetterString_Regular.sln
echo   - 单头文件版本: build_vs2022_single\BetterString_SingleHeader.sln
echo.

:end
echo 请使用Visual Studio 2022打开上述解决方案文件进行编译。
echo 每个解决方案中都有对应的示例项目，可用于测试库功能。 