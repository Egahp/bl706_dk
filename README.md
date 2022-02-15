# BL706 开源开发板

## 开发板引脚引出
![image](https://github.com/Egahp/bl706_dk/blob/master/picture/pinout.png)

## 搭建项目
    注:以下教程已经安装好基于 vscode + wsl2 的开发环境

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
# search_application(${CMAKE_SOURCE_DIR}/${APP_DIR})
# 注释上一行代码修改为下一行，用以使用外部的APP_DIR文件夹
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
    复制 bl_mcu_sdk/examples/gpio/gpio_blink 文件夹到 project 文件夹，wsl2 命令行进入bl_mcu_sdk 目录，执行以下命令，如果成功则修改完成，项目根文件夹会多出 output 文件夹以及 latest 文件夹

```bash
make APP=gpio_blink
```

### 第四步
    复制配置工具链时候的工具链文件夹内容到riscv-elf文件夹下，工具链文件夹一般名称为riscv64-unknown-elf-xxxxxx 或 riscv64-elf-xxxxxx 等字样

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
    1.打开dev cube下载编译成功的bin文件
    2.启动jlinkGdbServer，Target device 设置为RISC-V，little endian， jtag 接口 Fixed 12000 kHz，勾选init registers，不勾选 Localhost only。
    3.vscode 打开对应的main文件，按F5 开始调试

    注: 如果vscode 弹出连接超时，尝试更改ip地址
    


## [SDK](https://github.com/bouffalolab/bl_mcu_sdk)

## [开发指南以及环境配置](https://dev.bouffalolab.com/media/doc/sdk/bl_mcu_sdk_zh/index.html)

## [在线MCU配置网站](https://dev.bouffalolab.com/media/config/index.html)

## [更多资料请访问博流智能社区](https://dev.bouffalolab.com/home)
