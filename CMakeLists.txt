cmake_minimum_required(VERSION 3.10)
project(stc89_demo C)

# 导出编译命令给 C/C++ 扩展使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 自动收集所有 .c 文件
file(GLOB_RECURSE C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# 输出文件名
set(OUTPUT_NAME demo)

# 编译标志
set(SDCC_FLAGS -mmcs51 --std-c99 --model-small)
set(INCLUDE_DIRS 
    -I${CMAKE_CURRENT_SOURCE_DIR}/src 
    -I${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    -I/usr/share/sdcc/include/mcs51
)

# 生成 .rel 文件列表和编译命令
set(REL_FILES "")
set(COMPILE_COMMANDS "")

foreach(SRC ${C_SOURCES})
    get_filename_component(NAME_WE ${SRC} NAME_WE)
    set(REL_FILE "${CMAKE_CURRENT_BINARY_DIR}/${NAME_WE}.rel")
    list(APPEND REL_FILES ${REL_FILE})
    
    # 为每个 .c 文件添加编译命令
    list(APPEND COMPILE_COMMANDS
        COMMAND sdcc ${SDCC_FLAGS} ${INCLUDE_DIRS} -c ${SRC} -o ${REL_FILE}
    )
endforeach()

# 编译所有文件并链接
add_custom_target(demo ALL
    # 先编译所有 .c 文件为 .rel
    ${COMPILE_COMMANDS}
    
    # 然后链接所有 .rel 文件
    COMMAND sdcc ${SDCC_FLAGS} ${INCLUDE_DIRS} 
            --no-dead-code
            ${REL_FILES}
            -o ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.ihx
    
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Compiling with SDCC for MCS51..."
    VERBATIM
)

# 转换为 .hex
add_custom_command(TARGET demo POST_BUILD
    COMMAND packihx ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.ihx > ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.hex || true
    COMMENT "Converting .ihx to .hex"
)

# 烧录目标
add_custom_target(flash
    COMMAND /home/hysin/miniconda3/bin/stcgal -P stc89 -b 115200 -p /dev/ttyUSB0 ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.ihx
    DEPENDS demo
    COMMENT "Flashing firmware to STC89C516RD+..."
)
