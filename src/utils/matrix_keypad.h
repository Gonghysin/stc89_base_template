/*===========================================================================
 * matrix_keypad.h - 4x4矩阵键盘驱动模块
 * 
 * 描述：实现矩阵键盘的逐列扫描，带软件消抖
 * 硬件连接：
 *   - 列线：P1.0~P1.3 (P10~P13) - 输出
 *   - 行线：P1.4~P1.7 (P14~P17) - 输入（内部上拉）
 *===========================================================================*/

#ifndef MATRIX_KEYPAD_H
#define MATRIX_KEYPAD_H

#include <mylib.h>

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
#define KEYPAD_PORT P1

// 列掩码 （P1.0 ~ P1.3)
#define COL_MASK 0x0F  // 0000 1111
// 行掩码  (P1.4~P1.7)
#define ROW_MASK 0xF0 // 1111 0000

// 无按键时的返回值
#define NO_KEY 0xFF

/// @brief 初始化矩阵键盘
/// @param  设置列为输出（高电平），行为输入（上拉）
void keypad_init(void);




/**
 * @brief 扫描矩阵键盘（带消抖）
 * @return 返回按键位置（0~15），如果无按键按下返回 NO_KEY (0xFF)
 * @note 按键编号：S1=0, S2=1, S3=2, ..., S16=15
 *       位置计算：key_pos = row * 4 + col
 */
unsigned char keypad_scan(void);




#endif