cmake_minimum_required(VERSION 3.10)
project(stc89_demo C)

# 源文件
set(SOURCES 
    src/main.c
    src/mylib.c
    src/utils/seg_display.c
)

# 输出文件名
set(OUTPUT_NAME demo)

# 自定义目标来编译 SDCC 项目
add_custom_target(demo ALL
    # 先编译 seg_display.c
    COMMAND sdcc -mmcs51 --std-c99 --model-small -c -I${CMAKE_CURRENT_SOURCE_DIR}/src -I${CMAKE_CURRENT_SOURCE_DIR}/src/utils -o ${CMAKE_CURRENT_BINARY_DIR}/seg_display.rel ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/seg_display.c
    # 编译 mylib.c
    COMMAND sdcc -mmcs51 --std-c99 --model-small -c -I${CMAKE_CURRENT_SOURCE_DIR}/src -I${CMAKE_CURRENT_SOURCE_DIR}/src/utils -o ${CMAKE_CURRENT_BINARY_DIR}/mylib.rel ${CMAKE_CURRENT_SOURCE_DIR}/src/mylib.c
    # 编译 main.c 并链接所有 .rel 文件
    COMMAND sdcc -mmcs51 --std-c99 --model-small -I${CMAKE_CURRENT_SOURCE_DIR}/src -I${CMAKE_CURRENT_SOURCE_DIR}/src/utils -o ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.ihx ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c ${CMAKE_CURRENT_BINARY_DIR}/mylib.rel ${CMAKE_CURRENT_BINARY_DIR}/seg_display.rel
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Compiling with SDCC for MCS51..."
    SOURCES ${SOURCES}
)

# 自定义命令：转换 .ihx 到 .hex (可选)
add_custom_command(TARGET demo POST_BUILD
    COMMAND packihx ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.ihx > ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.hex || true
    COMMENT "Converting .ihx to .hex (optional)"
)

# 添加烧录目标 (方便直接 make flash)
# -P stc89: 强制指定协议为 STC89 系列 (对于老芯片建议加上)
# -p /dev/ttyUSB0: 你的 USB 转串口设备路径，请根据实际情况修改
add_custom_target(flash
    COMMAND /home/hysin/miniconda3/bin/stcgal -P stc89 -p /dev/ttyUSB0 ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.ihx
    DEPENDS demo
    COMMENT "Flashing firmware to STC89C516RD+..."
)
