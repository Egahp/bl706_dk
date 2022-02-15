# 需要的库名称
# set(TARGET_REQUIRED_LIBS <your_lib> ...)

# 自定义链接脚本
# set(LINKER_SCRIPT <your_linker_script.ld>)

# 添加需要的头文件路径
set(TARGET_REQUIRED_PRIVATE_INCLUDE 
# ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# 添加要编译的源文件
set(TARGET_REQUIRED_SRCS 
${MIDDLEWARE_DIR}/segger_rtt/segger_rtt_debug_log.c
${MIDDLEWARE_DIR}/segger_rtt/SEGGER_RTT_printf.c
${MIDDLEWARE_DIR}/segger_rtt/SEGGER_RTT.c

# ${CMAKE_CURRENT_SOURCE_DIR}/source.c
)

set(mains main.c)

generate_bin()
