#include "buzzer.h"
#include "timer.h"


/// @brief 使蜂鸣器发出持续一定时间、一定频率的声音。  
/// 使用前必须执行 Timer0_Init();
/// @param hz 发出声音的频率
/// @param ms 声音持续的时间
void buzzer_ring_period(u16 hz, u16 ms)
{
    
    u16 delay_val; // 翻转延时参数（单位 ms）
    u32 total_toggles;

    if (hz==0) return;

    // 计算 delay 所需要的参数。
    // T = 1/hz (s)= 1000/hz ms
    // 半周期 L = T / 2 = 500/hz ms
    delay_val = 500 / hz;

    // 通过时长来计算要翻转多少次io口
    total_toggles = (u32)ms * hz * 2 / 1000; // 蜂鸣器总共需要翻转的次数

    while(total_toggles--)
    {
        BEEP = !BEEP;
        delay_ms_timer(delay_val);
    }
}



/// @brief 发出连续的指定频率的蜂鸣直到enable变为0；
/// @param enable 传入的u8指针
/// @param hz 要发出的频率
void buzzer_ring_continued(u8 *enable, u16 hz)
{
    u16 delay_val = 500 / hz;
    while(enable)
    {
        BEEP = !BEEP;
        delay_ms_timer(delay_val);
    }
}