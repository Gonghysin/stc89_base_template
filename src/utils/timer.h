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

/**
 * @brief 基于Timer0的毫秒级延时函数
 * @param ms 延时的毫秒数
 * @note 需要先调用Timer0_Init()初始化定时器
 */
void delay_ms_timer(unsigned int ms);

/**
 * @brief 设置定时器0的回调函数
 * @param callback 回调函数指针（无参数无返回值）
 */
void Timer0_SetCallback(void (*callback)(void));

// void Timer0_ISR(void) __interrupt(1);

extern volatile unsigned int timer_delay_counter;

#endif
