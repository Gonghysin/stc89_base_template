/**
 * @file key.c
 * @brief 轮询式按键识别模块
 * @note 提供阻塞式和非阻塞式两种按键检测方式
 */

#include "key.h"
#include "timer.h"

// 保存上一次的按键状态（用于边沿检测）
static unsigned char last_key_state[4] = {1, 1, 1, 1};

/**
 * @brief 读取指定按键的当前电平状态
 * @param key_num 按键编号（1-4）
 * @return 0=按下，1=释放
 */
unsigned char key_get_state(unsigned char key_num)
{
    switch(key_num)
    {
        case 1: return P3_1;
        case 2: return P3_0;
        case 3: return P3_2;
        case 4: return P3_3;
        default: return 1;
    }
}

/**
 * @brief 阻塞式按键扫描（等待按键释放）
 * @return 按键编号1-4，无按键返回0
 */
unsigned char key_scan(void)
{
    unsigned char key_num = 0;
    
    // 检测按键1 (P3.1)
    if (P3_1 == 0) {
        delay_ms_timer(20);  // 消抖
        if (P3_1 == 0) {     // 确认按下
            key_num = 1;
        }
    }
    
    // 检测按键2 (P3.0)
    if (P3_0 == 0) {
        delay_ms_timer(20);
        if (P3_0 == 0) {
            key_num = 2;
        }
    }
    
    // 检测按键3 (P3.2)
    if (P3_2 == 0) {
        delay_ms_timer(20);
        if (P3_2 == 0) {
            key_num = 3;
        }
    }
    
    // 检测按键4 (P3.3)
    if (P3_3 == 0) {
        delay_ms_timer(20);
        if (P3_3 == 0) {
            key_num = 4;
        }
    }
    
    // 如果检测到按键，等待释放
    if (key_num != 0) {
        // 等待按键释放
        while (key_get_state(key_num) == 0);
        delay_ms_timer(20);  // 释放后消抖
    }
    
    return key_num;
}

/**
 * @brief 非阻塞式按键检测（边沿触发）
 * @return 按键编号1-4，无新按键按下返回0
 * @note 只在按键从释放到按下的瞬间返回按键号，需持续调用
 */
unsigned char key_get_press(void)
{
    unsigned char i;
    unsigned char current_state;
    static unsigned char debounce_cnt[4] = {0, 0, 0, 0};
    
    // 扫描所有按键
    for (i = 0; i < 4; i++)
    {
        current_state = key_get_state(i + 1);
        
        // 检测下降沿（从1到0，即按键按下）
        if (current_state == 0 && last_key_state[i] == 1)
        {
            debounce_cnt[i]++;
            if (debounce_cnt[i] >= 2)  // 简单的消抖计数
            {
                last_key_state[i] = 0;
                debounce_cnt[i] = 0;
                return i + 1;  // 返回按键编号
            }
        }
        // 检测上升沿（从0到1，即按键释放）
        else if (current_state == 1 && last_key_state[i] == 0)
        {
            last_key_state[i] = 1;
            debounce_cnt[i] = 0;
        }
        // 状态未改变，重置消抖计数
        else if (current_state == last_key_state[i])
        {
            debounce_cnt[i] = 0;
        }
    }
    
    return 0;  // 无按键事件
}


