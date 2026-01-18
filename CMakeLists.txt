cmake_minimum_required(VERSION 3.10)
project(stc89_demo C)

# 导出编译命令给 C/C++ 扩展使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 输出文件名
set(OUTPUT_NAME demo)

# 编译标志
set(SDCC_FLAGS -mmcs51 --std-c99 --model-small --stack-auto --opt-code-size)
set(INCLUDE_DIRS 
    -I${CMAKE_CURRENT_SOURCE_DIR}/src 
    -I${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    -I/usr/share/sdcc/include/mcs51
)

# 函数：递归查找 .c 文件的所有依赖
function(find_dependencies SOURCE_FILE RESULT_VAR)
    set(DEPS "")
    set(TO_PROCESS "${SOURCE_FILE}")
    set(PROCESSED "")
    
    while(TO_PROCESS)
        list(GET TO_PROCESS 0 CURRENT_FILE)
        list(REMOVE_AT TO_PROCESS 0)
        
        # 如果已处理过，跳过
        list(FIND PROCESSED "${CURRENT_FILE}" FOUND_IDX)
        if(NOT FOUND_IDX EQUAL -1)
            continue()
        endif()
        
        list(APPEND PROCESSED "${CURRENT_FILE}")
        
        # 如果是 .c 文件，添加到依赖列表
        if(EXISTS "${CURRENT_FILE}" AND CURRENT_FILE MATCHES "\\.c$")
            list(APPEND DEPS "${CURRENT_FILE}")
        endif()
        
        # 读取文件内容，查找 #include
        if(EXISTS "${CURRENT_FILE}")
            file(READ "${CURRENT_FILE}" FILE_CONTENT)
            string(REGEX MATCHALL "#include[ \t]+\"([^\"]+)\"" INCLUDES "${FILE_CONTENT}")
            
            foreach(INCLUDE_LINE ${INCLUDES})
                string(REGEX REPLACE "#include[ \t]+\"([^\"]+)\"" "\\1" HEADER_FILE "${INCLUDE_LINE}")
                
                # 在搜索路径中查找头文件
                set(HEADER_PATH "")
                foreach(SEARCH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src/utils")
                    if(EXISTS "${SEARCH_DIR}/${HEADER_FILE}")
                        set(HEADER_PATH "${SEARCH_DIR}/${HEADER_FILE}")
                        break()
                    endif()
                endforeach()
                
                # 如果找到头文件，添加到待处理列表
                if(HEADER_PATH)
                    list(APPEND TO_PROCESS "${HEADER_PATH}")
                    
                    # 查找对应的 .c 文件
                    string(REGEX REPLACE "\\.h$" ".c" C_FILE "${HEADER_PATH}")
                    if(EXISTS "${C_FILE}")
                        list(APPEND TO_PROCESS "${C_FILE}")
                    endif()
                endif()
            endforeach()
        endif()
    endwhile()
    
    list(REMOVE_DUPLICATES DEPS)
    set(${RESULT_VAR} ${DEPS} PARENT_SCOPE)
endfunction()

# 从 main.c 开始查找所有依赖
set(MAIN_C "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
find_dependencies("${MAIN_C}" C_SOURCES)

message(STATUS "编译以下文件:")
foreach(SRC ${C_SOURCES})
    message(STATUS "  ${SRC}")
endforeach()

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
