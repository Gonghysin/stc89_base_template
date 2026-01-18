/**
 * @file timer.c
 * @brief 定时器0模块实现
 */

#include "timer.h"

// 定时器初值：12T单片机，1ms定时
// 实测微调值（补偿中断处理延迟）
#define T1MS_H 0xFC
#define T1MS_L 0x68

// 导出给main.c中的中断函数使用
volatile unsigned int timer_delay_counter = 0;

void delay_ms_timer(unsigned int ms)
{

    timer_delay_counter = ms;
    while (timer_delay_counter > 0)
    {
        /* code */
    }
    
}



// 在文件顶部声明全局函数指针
static void (*timer_callback)(void) = 0;

// 提供注册回调的函数
void Timer0_SetCallback(void (*callback)(void))
{
    timer_callback = callback;
}

// // 定时器0中断服务程序
void Timer0_ISR(void) __interrupt(1)
{
    // static unsigned char cnt = 0;
    
    TH0 = 0xFC;
    TL0 = 0x68;
    
    // 调用注册的回调函数
    if(timer_callback != 0) {
        timer_callback();
    }

    if(timer_delay_counter > 0)
    {
        timer_delay_counter--;
    }
}





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


// void Timer0_Routine(void (*function)(unsigned int* T0Count)) __interrupt(1)
// {
//     static unsigned int T0Count;
//     TL0 = 0x18;
//     TH0 = 0xFC;

//     function(&T0Count);
// }
