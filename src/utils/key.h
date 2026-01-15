/**
 * @file key.h
 * @brief 轮询式按键识别模块
 * @note 硬件连接：按键按下时引脚接GND（低电平0表示按下）
 */

#ifndef __KEY_H__
#define __KEY_H__

#include <8052.h>

// 按键编号定义
#define KEY1  1
#define KEY2  2
#define KEY3  3
#define KEY4  4

// 按键状态定义
#define KEY_PRESSED  0  // 按下
#define KEY_RELEASED 1  // 释放

/**
 * @brief 扫描所有按键并返回按下的按键编号（阻塞式，带消抖）
 * @return 按键编号（1-4），无按键按下返回0
 * @note 此函数会等待按键释放后才返回，适合简单应用
 */
unsigned char key_scan(void);

/**
 * @brief 非阻塞式按键扫描，检测按键按下事件（边沿触发）
 * @return 按键编号（1-4），无新按键按下返回0
 * @note 需要在主循环中持续调用，只在按键从释放到按下的瞬间返回按键号
 */
unsigned char key_get_press(void);

/**
 * @brief 读取指定按键的当前状态（电平状态）
 * @param key_num 按键编号（1-4）
 * @return KEY_PRESSED(0)或KEY_RELEASED(1)
 */
unsigned char key_get_state(unsigned char key_num);

#endif