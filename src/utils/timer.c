/**
 * @file timer.c
 * @brief 定时器0模块实现
 */

#include "timer.h"

// 定时器初值：12T单片机，1ms定时
// 实测微调值（补偿中断处理延迟）
#define T1MS_H 0xFC
#define T1MS_L 0x68

void Timer0_Init(void)
{
    TMOD &= 0xF0;
    TMOD |= 0x01;
    TH0 = T1MS_H;
    TL0 = T1MS_L;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}
