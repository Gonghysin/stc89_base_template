/*===========================================================================
 * seg_display.c - 数码管显示控制实现
 *===========================================================================*/

 #include <8052.h>
 #include "seg_display.h"
 #include "seg_table.h"
 #include "mylib.h"

// 段码表定义
__code unsigned char SEG_CODE[10] = {
    0x3F,  // 0: 0011 1111
    0x06,  // 1: 0000 0110
    0x5B,  // 2: 0101 1011
    0x4F,  // 3: 0100 1111
    0x66,  // 4: 0110 0110
    0x6D,  // 5: 0110 1101
    0x7D,  // 6: 0111 1101
    0x07,  // 7: 0000 0111
    0x7F,  // 8: 0111 1111
    0x6F   // 9: 0110 1111
};


unsigned char display_buffer[SEG_NUM] = {
    SEG_BLANK, SEG_BLANK, SEG_BLANK, SEG_BLANK,
    SEG_BLANK, SEG_BLANK, SEG_BLANK, SEG_BLANK
 };

 /*---------------------------------------------------------------------------
 * 硬件接口宏定义
 *---------------------------------------------------------------------------*/
#define SEG_DATA P0          // 段码输出（74HC245）
#define SEG_SELECT P2        // 位选输出（74HC138，使用P2.2-P2.4）
#define SEG_SELECT_MASK 0xE3 // P2其他位保护掩码

/*---------------------------------------------------------------------------
 * 基础函数实现
 *---------------------------------------------------------------------------*/

 void seg_init(void) {
    seg_clear_all();
 }

 void seg_set_digit(unsigned char pos, unsigned char num) {
    if(pos >= SEG_NUM) return;

    if(num <= 9) {
        display_buffer[pos] = SEG_CODE[num]; // 显示数字
    } else {
        display_buffer[pos] = num; // 直接显示段码 （ 用于特殊字符 ）
    }
 }

 void seg_set_digit_dot(unsigned char pos, unsigned char num, unsigned char dot) {
    if(pos >= SEG_NUM || num > 9) return;

    display_buffer[pos] = SEG_CODE[num];
    /**
     * @brief 设置指定位置的小数点显示
     * 
     * 如果dot参数为真(非零),则在显示缓冲区的指定位置设置小数点段。
     * 通过将SEG_DOT标志位与当前显示内容进行或运算来实现。
     * 
     * @param dot 小数点显示标志,非零值表示显示小数点
     * @param pos 显示缓冲区中的位置索引
     * 
     * @note 使用按位或操作(|=)确保不会影响该位置已有的其他段显示
     */
    if(dot) {
        display_buffer[pos] |= SEG_DOT;;
    }
 }

void seg_clear_digit(unsigned char pos) {
    if(pos >= SEG_NUM) return;
    display_buffer[pos] = SEG_BLANK;
}

void seg_clear_all(void) {
    unsigned char i;
    for(i = 0; i <SEG_NUM; i++){
        display_buffer[i] = SEG_BLANK;
    }
}

/**
 * @brief 数码管扫描显示函数
 * 
 * 该函数用于实现数码管的动态扫描显示。通过依次选择不同的数码管位，
 * 并显示对应位置缓冲区中的数据，利用人眼视觉暂留效应实现多位数码管的显示。
 * 
 * @note 该函数需要周期性调用以维持数码管的显示效果
 * @note 使用静态变量 scan_pos 保持当前扫描位置，每次调用自动切换到下一位
 * 
 * 工作流程：
 * 1. 通过 SEG_SELECT 选择当前要显示的数码管位（使用位移操作）
 * 2. 将 display_buffer[scan_pos] 中的数据输出到 SEG_DATA
 * 3. 延时 20ms 以保持显示稳定
 * 4. scan_pos 递增，循环扫描所有数码管位（0 到 SEG_NUM-1）
 * 5. 扫描完成后清空显示数据（SEG_BLANK）以避免重影
 * 
 * @warning 代码中 delay_ms(20) 后缺少分号，需要修正
 */
void seg_scan(void) {
    static unsigned char scan_pos = 0; // 当前扫描位置

    unsigned char reversed_pos = (SEG_NUM - 1) - scan_pos;

    SEG_SELECT = (SEG_SELECT & SEG_SELECT_MASK) | (reversed_pos << 2);

    SEG_DATA = display_buffer[scan_pos];

    delay_ms(2);

    scan_pos++;
    if(scan_pos >= SEG_NUM) {
        scan_pos = 0;
    }

    SEG_DATA = SEG_BLANK;
    
}


/*---------------------------------------------------------------------------
 * 高级函数实现
 *---------------------------------------------------------------------------*/


 void seg_show_number(unsigned long num, unsigned char start_pos) {
    unsigned char i;
    unsigned char digits[10];
    unsigned char digit_count = 0;

    // 特殊处理0
    if(num == 0) {
        seg_set_digit(start_pos, 0);
        return;
    }

    // 提取各位数字
    while(num > 0 && digit_count < 10) {
        digits[digit_count++] = num % 10;
        num /= 10;
    }

    // 从右边到左边显示（低位在右）
    for(i = 0; i < digit_count; i++){
        if((start_pos + digit_count -1 -i) <SEG_NUM) {
            seg_set_digit(start_pos + digit_count -1-i, digits[i]);
        }
    }
 }

 void seg_show_signed(long num, unsigned char start_pos) {
    if(num < 0) {
        display_buffer[start_pos] = SEG_MINUS; // 显示符号
        seg_show_number(-num, start_pos + 1);
    } else {
        seg_show_number(num, start_pos);
    }
 }