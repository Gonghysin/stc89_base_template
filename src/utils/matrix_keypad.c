#include "matrix_keypad.h"
#include "mylib.h"
#include "timer.h"


// 列扫描码（P1.3→列1, P1.2→列2, P1.1→列3, P1.0→列4）
static const unsigned char col_scan[KEYPAD_COLS] = {
    0xF7,  // 1111 0111 - 扫描第1列 (P1.3 = 0) → S1, S5, S9, S13
    0xFB,  // 1111 1011 - 扫描第2列 (P1.2 = 0) → S2, S6, S10, S14
    0xFD,  // 1111 1101 - 扫描第3列 (P1.1 = 0) → S3, S7, S11, S15
    0xFE   // 1111 1110 - 扫描第4列 (P1.0 = 0) → S4, S8, S12, S16
};

// 行扫描码（P1.7→行1, P1.6→行2, P1.5→行3, P1.4→行4）
static const unsigned char row_scan[KEYPAD_ROWS] = {
    0x70,  // 0111 0000 - 第1行 (P1.7 = 0) → S1, S2, S3, S4
    0xB0,  // 1011 0000 - 第2行 (P1.6 = 0) → S5, S6, S7, S8
    0xD0,  // 1101 0000 - 第3行 (P1.5 = 0) → S9, S10, S11, S12
    0xE0   // 1110 0000 - 第4行 (P1.4 = 0) → S13, S14, S15, S16
};


void keypad_init(void)
{
    KEYPAD_PORT = 0xFF;
}

unsigned char keypad_scan(void)
{
    unsigned char col, row;
    unsigned char key_value;

    //逐列扫描
    for (col = 0; col < KEYPAD_COLS; col++) {
        // 设置当前列为0
        KEYPAD_PORT = col_scan[col];

        delay_ms_timer(1);

        key_value = KEYPAD_PORT;

        // 检测是否有按键按下
        if ((key_value & ROW_MASK) != ROW_MASK) {
            // 消抖
            delay_ms_timer(10);

            KEYPAD_PORT = col_scan[col];  // 重新设置当前列
            delay_ms_timer(1);                   // 短暂延时


            key_value = KEYPAD_PORT;
            if ((key_value & ROW_MASK) != ROW_MASK) {
                // 确认有按键按下了，接下来需要找出来是哪个按键被按下
                for (row = 0; row < KEYPAD_ROWS; row++) {
                    if ((key_value & ROW_MASK) == row_scan[row]) {
                        // 等待按键释放
                        while ((KEYPAD_PORT & ROW_MASK) != ROW_MASK);
                        delay_ms_timer(10); //消抖

                        // 恢复所有列为高电平
                        KEYPAD_PORT = 0xFF;

                        // 返回值
                        return row *KEYPAD_COLS + col;
                    }
                }
            }

        }


    }
    // 恢复所有列为高电平
    KEYPAD_PORT = 0xFF;
    
    // 无按键按下
    return NO_KEY;
}
