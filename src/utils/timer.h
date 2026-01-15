/**
 * @file timer.h
 * @brief 定时器0模块
 * @details 提供Timer0的初始化功能
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include <mylib.h>

// 定时器初值常量
#define T1MS_H 0xFC
#define T1MS_L 0x68

/**
 * @brief 初始化Timer0为1ms定时中断模式
 * @note 12T单片机@11.0592MHz
 */
void Timer0_Init(void);

#endif
