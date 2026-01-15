#include "LCD1602.h"
#include "mylib.h"




void LCD_Delay(unsigned int t)
{
    while (t--);
    
}

void LCD_WriteCommand(unsigned char cmd) {
    LCD_RS = 0;
    LCD_WR = 0;  // 修改：LCD_RW -> LCD_WR
    LCD_DATA = cmd;
    LCD_EN = 1;
    LCD_Delay(10);
    LCD_EN = 0;
    LCD_Delay(10);
}

void LCD_WriteData(unsigned char dat) {
    LCD_RS = 1;
    LCD_WR = 0;
    LCD_DATA = dat;
    /**
     * @brief 使能LCD1602的使能信号(Enable Signal)
     * 
     * 将LCD的EN(Enable)引脚设置为高电平(1),用于触发LCD执行数据读写操作。
     * 在LCD1602的时序中,EN引脚的上升沿或下降沿会触发LCD锁存数据总线上的数据。
     * 
     * @note 通常需要配合LCD_EN = 0使用,形成一个完整的使能脉冲
     * @note 在设置EN为高电平后,需要适当的延时以满足LCD的时序要求
     */
    LCD_EN = 1;
    LCD_Delay(10);
    LCD_EN = 0;
    LCD_Delay(10);
}


void LCD_SetCursor(unsigned char row, unsigned char col) {
    if(row == 1) {
        LCD_WriteCommand(0x80 |(col -1));
    } else if(row == 2) {
        LCD_WriteCommand(0x80 | (col - 1 + (0x40)));
    }
}

// init
void LCD_Init(void) {
    LCD_Delay(15000);  // 增加启动延时
    LCD_WriteCommand(0x38);  // 8位数据接口，2行显示，5x7点阵
    LCD_Delay(500);
    LCD_WriteCommand(0x38);
    LCD_Delay(500);
    LCD_WriteCommand(0x38);
    LCD_Delay(500);
    LCD_WriteCommand(0x0C);  // 显示开，光标关
    LCD_Delay(500);
    LCD_WriteCommand(0x06);  // 光标右移
    LCD_Delay(500);
    LCD_WriteCommand(0x01);  // 清屏
    LCD_Delay(2000);
}


void LCD_ShowChar(unsigned char row, unsigned char col, char c) {
    LCD_SetCursor(row, col);
    LCD_WriteData(c);
}

void LCD_ShowString(unsigned char row, unsigned char col, char *str) {
    LCD_SetCursor(row, col);
    while(*str) {
        LCD_WriteData(*str++);
    }
}

unsigned int LCD_Pow(unsigned int x, unsigned char y){
    unsigned int result = 1;
    while(y--) {
        result *= x;
    }
    return result;
}

void LCD_ShowNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len) {
    unsigned char i;
    LCD_SetCursor(row, col);
    for(i = len; i > 0; i--){
        LCD_WriteData('0' + num / LCD_Pow(10, i - 1) % 10);
    }
}

void LCD_ShowSignedNum(unsigned char row, unsigned char col, int num, unsigned char len) {
    unsigned char i;
    unsigned int numl;
    LCD_SetCursor(row, col);
    if(num > 0) {
        LCD_WriteData('+');
        numl = num;
    } else {
        LCD_WriteData('-');
        numl = -num;
    }
    for(i = len; i > 0; i--) {
        LCD_WriteData('0' + numl / LCD_Pow(10, i - 1) % 10 );
    }
}

void LCD_ShowHexNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len) {
    unsigned char i, singleNum;
    LCD_SetCursor(row, col);
    for(i = len; i > 0; i--) {
        singleNum = num / LCD_Pow(16, i - 1) % 16;
        if(singleNum < 10) {
            LCD_WriteData('0' + singleNum);
        } else {
            LCD_WriteData('A' + singleNum - 10);
        }
    }
}

void LCD_ShowBinNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len) {
    unsigned char i;
    LCD_SetCursor(row, col);
    for(i = len; i > 0; i--) {
        LCD_WriteData('0' + num / LCD_Pow(2, i-1) % 2);
    }
}

/**
 * @brief 将有符号整数转换为字符串
 * @param num 要转换的有符号整数（范围：-32768 到 32767）
 * @return 指向字符串的指针（静态缓冲区，不可重入）
 * @note 返回的字符串存储在静态缓冲区中，下次调用会覆盖
 */
char *tran_num_to_string(signed int num)
{
    static char buffer[7];  // 最大6位数字 + 符号 + '\0' = "-32768\0"
    char temp[7];
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned int abs_num;
    unsigned char is_negative = 0;
    
    // 处理负数
    if (num < 0) {
        is_negative = 1;
        abs_num = -num;
    } else {
        abs_num = num;
    }
    
    // 特殊情况：0
    if (abs_num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }
    
    // 将数字逆序存入临时缓冲区
    while (abs_num > 0) {
        temp[i++] = '0' + (abs_num % 10);
        abs_num /= 10;
    }
    
    // 添加符号（如果是负数）
    if (is_negative) {
        buffer[j++] = '-';
    }
    
    // 将临时缓冲区的内容反转到正式缓冲区
    while (i > 0) {
        buffer[j++] = temp[--i];
    }
    
    // 添加字符串结束符
    buffer[j] = '\0';
    
    return buffer;
}
