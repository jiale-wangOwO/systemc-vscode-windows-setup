# 使用VSCode开发SystemC保姆级安装教程（Windows环境）
SystemC是一种用于硬件系统级建模的C++库，官方提供了Windows环境使用Microsoft Visual C++的安装教程。但对于习惯使用VScode编辑器的小白开发者，配置SystemC环境可能令人头疼。本文使用msys2在Windows系统下安装SystemC，并在VSCode中配置开发环境，手把手带大家快速搭建SystemC开发环境。

## 1. 安装Visual Studio Code
VS Code是一款轻量级的开源代码编辑器，支持多种编程语言，拥有丰富的插件生态。

访问[VS Code官网](https://code.visualstudio.com/)(https://code.visualstudio.com/)下载并安装。安装时建议勾选"Add to PATH"选项，方便以后在终端中直接使用`code`命令打开VS Code。

## 2. 安装MSYS2
MSYS2是一个在Windows上提供类Unix环境的软件包管理器，可以方便地安装编译工具链
- 安装步骤：
  1. 访问[MSYS2官网](https://www.msys2.org/)(https://www.msys2.org/)下载安装程序
  2. 安装路径建议保持默认（`C:\msys64`）（建议路径不要有中文和空格）
  3. 安装完成后打开 **MSYS2 MSYS** 终端（开始菜单搜索，上一步安装好有多个程序，注意名字不要进错了）
  4. 在终端中执行以下命令更新软件包数据库：
```bash
pacman -Syu  
```
（建议安装全程科学上网，否则可能会出现下载失败的情况）。

如果提示关闭终端，按提示操作后重新打开终端重新输入命令直至更新完成。

## 3. 安装编译工具链
  - **工具说明**：
    - `mingw-w64-x86_64-gcc`：GCC编译器套件，包含C、C++、Fortran等编译器，用于将源代码编译成可执行文件。
    - `mingw-w64-x86_64-cmake`：CMake用于自动化构建过程，用于生成Makefile文件，简化编译过程。
    - `mingw-w64-x86_64-make`：Make是一个自动化的工程管理工具，通过读取Makefile文件中的规则来自动化编译、链接等步骤。

- 依次执行以下命令：
  ```bash
  pacman -S mingw-w64-x86_64-gcc 
  ```
  ```bash
  pacman -S mingw-w64-x86_64-cmake 
  ```
  ```bash
  pacman -S mingw-w64-x86_64-make  
  ```
- 安装成功后，关闭msys2终端。

## 4. 配置环境变量
让系统识别编译工具的位置
- 操作步骤：
  1. 右键「此电脑」→「属性」→「高级系统设置」
  2. 在「环境变量」→「环境变量」中找到`Path`变量，添加：
     ```
     C:\msys64\mingw64\bin
     C:\msys64\usr\bin
     ```
     如果安装在其他路径，请修改为实际路径
  3. 验证：打开CMD输入 `gcc --version` 显示出GCC版本信息，则安装正确。


## 5. 获取SystemC源代码
- 访问[Accellera官网](https://accellera.org/)(https://accellera.org/)下载最新版本压缩包
- 建议解压到c盘，路径不要有中文和空格。本文以解压后路径 `C:\systemc-3.0.0\`为例。

## 6. CMake配置
- **为什么需要CMake**：自动处理编译依赖，生成适合的构建系统
- **操作步骤**：
  1. 打开 **MSYS2 MinGW 64-bit** 终端（注意不是MSYS终端）
  2. 创建并进入build目录（便于管理构建文件）：
     ```bash
     cd /c/systemc-3.0.0  # 进入SystemC源码目录
     mkdir build   # 创建独立的构建目录
     cd build      # 进入构建目录
     ```
  3. 生成构建配置：
    ```bash
    # 生成构建配置，指定调试模式、C++17标准、禁用C++扩展，并设置安装目录
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_EXTENSIONS=OFF -DCMAKE_INSTALL_PREFIX="C:/systemc-3.0.0/build" ..
    ```

## 7. 构建与安装
- **编译SystemC库**
```bash
cmake --build . --config Debug 
```

- **运行测试用例，不应该报错**
```bash
cmake --build . --config Debug --target examples/check
```

- **安装SystemC库，库将被安装到C:/systemc-3.0.0/install**
```bash
cmake --build . --config Debug --target install
```
截止到这一步，SystemC库已经安装到了C:/systemc-3.0.0/install目录下。可以关闭MSYS2 MinGW 64-bit终端。

## 8. VS Code开发环境配置
从这一步开始，我们将使用VS Code编辑器进行SystemC程序开发。下面通过hello world程序演示如何配置VS Code环境。

- **安装C/C++插件**

在VS Code左侧Extensions中搜索`C/C++`，安装 `'C/C++'`, `'C++ Themes'（可选）`插件。
- **打开项目文件夹**

自建项目空文件夹，例如`D:\Projects\HelloWorld_prj`

vscode打开文件夹的方法：左上角`file`->`open folder`->选择项目文件夹

- **创建程序文件**

在左侧explorer中的HelloWorld_prj文件夹中，创建新文件`hello.cpp`，内容如下：
```cpp
#include <systemc.h>

int sc_main(int argc, char* argv[]) {
  sc_start(); // 启动SystemC仿真
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
```

- **配置tasks.json（编译任务）**
  
在项目文件夹中创建`.vscode`文件夹，用于存放VS Code配置文件。

在`.vscode`文件夹中创建`tasks.json`文件，内容如下，注意修改路径后删除注释（json不支持注释）：

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: g++.exe build active file", 
            "command": "C:\\msys64\\mingw64\\bin\\g++.exe",  // 使用MinGW的g++，路径根据msys2实际安装路径修改
            "args": [
                "-fdiagnostics-color=always",  
                "-g",           
                "-I", "C:/systemc-3.0.0/install/include",  // SystemC头文件路径，根据systemc安装路径修改
                "-L", "C:/systemc-3.0.0/install/lib",  // SystemC库文件路径，根据systemc安装路径修改
                "-lsystemc",  
                "${file}",      
                "-o",       
                "${fileDirname}\\${fileBasenameNoExtension}.exe"
            ],
            "options": {
                "cwd": "${fileDirname}" 
            },
            "problemMatcher": ["$gcc"],
            "group": {
                "kind": "build",
                "isDefault": true  
            },
            "detail": "Generated task by Debugger."
        }
    ]
}
```
- **配置c_cpp_properties.json（可选，代码智能提示）**


在`.vscode`文件夹中创建`c_cpp_properties.json`文件，内容如下，注意修改路径后删除注释：
```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/systemc-3.0.0/install/include"   // SystemC头文件路径，根据systemc安装路径修改
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "compilerPath": "C:/msys64/mingw64/bin/g++.exe",  // 根据msys2 MinGW编译器路径修改
            "cStandard": "c17",
            "cppStandard": "gnu++17",   
            "intelliSenseMode": "windows-gcc-x64"
        }
    ],
    "version": 4
}
```

## 9. 编译和运行

在编辑器中切换到`hello.cpp`文件，Vs code上方的`Run`->`Run without debugging`，选择`C++ (GDB/LLDB)`->`C/C++: g++.exe build active file`，即可编译并运行程序。

预期输出：
```
Hello, World!
```

至此，SystemC开发环境配置完成。在新项目中，将`tasks.json`和`c_cpp_properties.json`复制到新项目的`.vscode`文件夹中，即可快速配置开发环境。