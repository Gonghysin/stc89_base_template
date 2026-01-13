/*===========================================================================
 * mylib.c - 个人代码库实现文件
 *===========================================================================*/

#include "mylib.h"

/*---------------------------------------------------------------------------
 * 延时函数实现
 *---------------------------------------------------------------------------*/

/**
 * @brief 延时指定的毫秒数
 * @param ms 延时时间（毫秒）
 * 
 * 实现说明：
 * - STC89C51RD @ 11.0592MHz (12T传统单片机)
 * - 使用 STC-ISP 软件生成的精确延时参数
 * - 精度：接近实际 1ms (误差约±5%)
 */
void delay_ms(unsigned int ms) {
    unsigned char i, j;
    
    // STC89C51RD (12T) @ 11.0592MHz
    // 根据实测调整的精确参数（原来慢3倍，现在修正）
    while (ms--) {
        _nop_();
        i = 1;
        j = 140;
        do {
            while (--j);
        } while (--i);
    }
}

/**
 * @brief 延时指定的微秒数
 * @param us 延时时间（微秒）
 * @note STC89C51RD @ 11.0592MHz (12T单片机)，适用于 us >= 14
 */
void delay_us(unsigned int us) {
    while (us--) {
        // 空循环体，编译器会保留循环
    }
}

/*---------------------------------------------------------------------------
 * 定时器相关函数
 *---------------------------------------------------------------------------*/

// 定时器中断计数器
static volatile unsigned int timer0_count = 0;

/**
 * @brief 初始化定时器0用于精确延时
 */
void timer0_init(void) {
    TMOD &= 0xF0;
    TMOD |= 0x01;
    
    TH0 = 0xFC;
    TL0 = 0x66;
    
    ET0 = 1;
    EA = 1;
    TR0 = 1;
    
    timer0_count = 0;
}

/**
 * @brief 定时器0中断服务函数
 */
void timer0_isr(void) __interrupt(1) {
    TH0 = 0xFC;
    TL0 = 0x66;
    timer0_count++;
}

/**
 * @brief 精确延时指定毫秒数
 */
void delay_ms_precise(unsigned int ms) {
    timer0_count = 0;
    while (timer0_count < ms) {
        // 等待定时器中断
    }
}

// 示例：
// void my_function(void) {
//     // 你的代码
// }
