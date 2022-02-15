# BL706 开源开发板

## 开发板引脚引出
![image](https://github.com/Egahp/bl706_dk/blob/master/picture/pinout.png)

***
## vscode + wsl2 + jlinkGdbServer 开发环境搭建教程
>注:以下教程已经安装好基于 vscode + wsl2 的开发环境，请先参照博流官方linux开发环境搭建教程
>使用wsl2和 vscode 搭建

### 第一步

 * 创建项目根文件夹并进入，执行以下git命令
 * project 用于存放不同工程的源文件，如prj1，prj2...
 * middleware 用于存放多个工程共用的中间件
 * riscv64_elf 用于复制一份交叉编译工具链用于调试
```
git init
git submodule add git@github.com:bouffalolab/bl_mcu_sdk.git

mkdir project
cd project
mkdir prj1
cd ../
mkdir middleware
mkdir riscv64_elf
```

### 第二步

 * 修改 bl_mcu_sdk/toos/tools.cmake 文件
```cmake
file(MAKE_DIRECTORY ${OUTPUT_DIR})
set(HEX_FILE ${OUTPUT_DIR}/${target_name}.hex)
set(BIN_FILE ${OUTPUT_DIR}/${target_name}.bin)
set(MAP_FILE ${OUTPUT_DIR}/${target_name}.map)
set(ASM_FILE ${OUTPUT_DIR}/${target_name}.asm)
# 找到以上这段代码，添加以下一行
set(ELF_FILE ${OUTPUT_DIR}/${target_name}.elf)
```
```cmake
add_custom_command(TARGET ${target_name}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${target_name}.elf> ${BIN_FILE}
    COMMAND ${CMAKE_OBJDUMP} -d -S $<TARGET_FILE:${target_name}.elf> >${ASM_FILE}
    # COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${mainname}.elf> ${HEX_FILE}
    COMMAND ${SIZE} $<TARGET_FILE:${target_name}.elf>
    COMMAND ${CMAKE_COMMAND} -E copy ${BIN_FILE} ${CMAKE_SOURCE_DIR}/tools/bflb_flash_tool/img/project.bin

    # 找到以上代码，添加下面 9 行，用于输出固定的文件以及输出 bin 文件
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_SOURCE_DIR}/../latest
    COMMAND ${CMAKE_COMMAND} -E copy ${BIN_FILE} ${CMAKE_SOURCE_DIR}/../latest/latest.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${ELF_FILE} ${CMAKE_SOURCE_DIR}/../latest/latest.elf
    COMMAND ${CMAKE_COMMAND} -E copy ${MAP_FILE} ${CMAKE_SOURCE_DIR}/../latest/latest.map
    COMMAND ${CMAKE_COMMAND} -E copy ${ASM_FILE} ${CMAKE_SOURCE_DIR}/../latest/latest.asm

    COMMAND ${CMAKE_COMMAND} -E copy ${BIN_FILE} ${CMAKE_SOURCE_DIR}/../output/${target_name}.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${ELF_FILE} ${CMAKE_SOURCE_DIR}/../output/${target_name}.elf
    COMMAND ${CMAKE_COMMAND} -E copy ${MAP_FILE} ${CMAKE_SOURCE_DIR}/../output/${target_name}.map
    COMMAND ${CMAKE_COMMAND} -E copy ${ASM_FILE} ${CMAKE_SOURCE_DIR}/../output/${target_name}.asm
```
 * 修改 bl_mcu_sdk/CMakeLists.txt 文件
```cmake
cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/tools/cmake/riscv64-unknown-elf-gcc.cmake)
include(${CMAKE_SOURCE_DIR}/drivers/${CHIP}_driver/cpu_flags.cmake)
include(${CMAKE_SOURCE_DIR}/tools/cmake/compiler_flags.cmake)
include(${CMAKE_SOURCE_DIR}/tools/cmake/tools.cmake)

PROJECT(${BOARD} C CXX ASM)
# set(CMAKE_VERBOSE_MAKEFILE ON)
ENABLE_LANGUAGE(ASM)

# 添加以下一行
set(MIDDLEWARE_DIR ${CMAKE_SOURCE_DIR}/../middleware)

include_directories(${CMAKE_SOURCE_DIR}/bsp/bsp_common/platform)

if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/bsp/board/${CHIP})
include_directories(${CMAKE_SOURCE_DIR}/bsp/board/${CHIP})
else()
message(FATAL_ERROR "${CMAKE_SOURCE_DIR}/bsp/board/${CHIP} is not exist")
endif()

# search_application(${CMAKE_SOURCE_DIR}/${APP_DIR})
# 注释上一行改为下一行
search_application(${CMAKE_SOURCE_DIR}/../${APP_DIR})
```
 * 修改 bl_mcu_sdk/Makefile 文件
```cmake
#common config
BOARD?=bl706_iot
CHIP?=bl702
APP_DIR?=project #修改默认 APP_DIR 为外部的project文件夹
APP?=helloworld 
CPU_ID?=none
```

### 第三步
 * 复制 bl_mcu_sdk/examples/gpio/gpio_blink 文件夹到 project 文件夹，wsl2 命令行进入bl_mcu_sdk 目录，执行以下命令，如果成功则修改完成，项目根文件夹会多出 output 文件夹以及 latest 文件夹

```bash
make APP=gpio_blink
```

### 第四步
 * 复制配置工具链时候的工具链文件夹内容到riscv-elf文件夹下，工具链文件夹一般名称为riscv64-unknown-elf-xxxxxx 或 riscv64-elf-xxxxxx 等字样

### 第五步

 * 添加launch.json

```json
{
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Bouffalo",
            "type": "cppdbg",
            "request": "launch",
            // 配置为你的  项目根目录/latest/latest.elf 文件路径
            "program": "/mnt/x/vector_project/vector_bl706/latest/latest.elf",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            // 配置为你的工具链
            "miDebuggerPath": "riscv64-unknown-elf-gdb",
            // 配置端口一般为2331，ip 地址为vEthernet(WSL) ipv4地址, 不行可以尝试localhost 或者本机ip地址 192.168.1.xxx之类的
            "miDebuggerServerAddress": "172.17.240.1:2331",
            "setupCommands": [
                {
                    "description": "为 gdb 启用整齐打印",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

### 第六步
 * 打开dev cube下载编译成功的bin文件
 * 启动jlinkGdbServer，Target device 设置为RISC-V，little endian， jtag 接口 Fixed 12000 kHz，勾选init registers，不勾选 Localhost only。
 * vscode 打开对应的main文件，按F5 开始调试

>注: 如果vscode 弹出连接超时，尝试更改ip地址
    

***
## 使用 jlink rtt view 打印log信息教程

### 第一步
 * 将 root/middleware/segger_rtt 文件夹复制到你的目录 middleware文件夹内
 * 将 root/project/segger_rtt 文件夹复制到你的目录 project 文件夹内

### 第二步
 * 修改bl_mcu_sdk/Makefile 文件
```cmake

#option config to use
SUPPORT_FLOAT?=n
SUPPORT_ROMAPI?=y
SUPPORT_HALAPI?=y
SUPPORT_USB_HS?=n
SUPPORT_HW_SEC_ENG_DISABLE?=n
SUPPORT_BLECONTROLLER_LIB?=n
# 添加以下一行
SEGGER_RTT_VIEW?=y

#cmake definition config
cmake_definition+= -DCHIP=$(CHIP)
cmake_definition+= -DCPU_ID=$(CPU_ID)
cmake_definition+= -DBOARD=$(BOARD)
cmake_definition+= -DAPP_DIR=$(APP_DIR)
cmake_definition+= -DAPP=$(APP)
cmake_definition+= -DBOOTROM=$(BOOTROM)
cmake_definition+= -DCONFIG_ROMAPI=$(SUPPORT_ROMAPI)
cmake_definition+= -DCONFIG_HALAPI=$(SUPPORT_HALAPI)
cmake_definition+= -DCONFIG_PRINT_FLOAT=$(SUPPORT_FLOAT)
cmake_definition+= -DCONFIG_USB_HS=$(SUPPORT_USB_HS)
cmake_definition+= -DCONFIG_HW_SEC_ENG_DISABLE=$(SUPPORT_HW_SEC_ENG_DISABLE)
cmake_definition+= -DCONFIG_BLECONTROLLER_LIB=$(SUPPORT_BLECONTROLLER_LIB)
# 添加以下一行
cmake_definition+= -DSEGGER_RTT_VIEW=$(SEGGER_RTT_VIEW)
```
 * 修改bl_mcu_sdk/CMakeLists.txt 文件
```cmake
cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/tools/cmake/riscv64-unknown-elf-gcc.cmake)
include(${CMAKE_SOURCE_DIR}/drivers/${CHIP}_driver/cpu_flags.cmake)
include(${CMAKE_SOURCE_DIR}/tools/cmake/compiler_flags.cmake)
include(${CMAKE_SOURCE_DIR}/tools/cmake/tools.cmake)

PROJECT(${BOARD} C CXX ASM)
# set(CMAKE_VERBOSE_MAKEFILE ON)
ENABLE_LANGUAGE(ASM)

# 添加以下一行
set(MIDDLEWARE_DIR ${CMAKE_SOURCE_DIR}/../middleware)

# 添加以下四行
if(SEGGER_RTT_VIEW)
add_definitions(-DSEGGER_RTT_VIEW)
include_directories(${MIDDLEWARE_DIR}/segger_rtt)
endif()

include_directories(${CMAKE_SOURCE_DIR}/bsp/bsp_common/platform)

if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/bsp/board/${CHIP})
include_directories(${CMAKE_SOURCE_DIR}/bsp/board/${CHIP})
else()
message(FATAL_ERROR "${CMAKE_SOURCE_DIR}/bsp/board/${CHIP} is not exist")
endif()

# search_application(${CMAKE_SOURCE_DIR}/${APP_DIR})
search_application(${CMAKE_SOURCE_DIR}/../${APP_DIR})
```
### 第三步
 * 编译工程
```bash
make APP=segger_rtt
```

***
## [SDK](https://github.com/bouffalolab/bl_mcu_sdk)

## [开发指南以及环境配置](https://dev.bouffalolab.com/media/doc/sdk/bl_mcu_sdk_zh/index.html)

## [在线MCU配置网站](https://dev.bouffalolab.com/media/config/index.html)

## [更多资料请访问博流智能社区](https://dev.bouffalolab.com/home)
